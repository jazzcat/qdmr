#include "d868uv_callsigndb.hh"
#include "utils.hh"
#include <QtEndian>

#define MAX_CALLSIGNS               0x00030d40  // Maximum number of callsings in DB (200k)

#define CALLSIGN_INDEX_BANK0        0x04000000  // Start address of the callsign index table
#define CALLSIGN_INDEX_BANK_OFFSET  0x00040000  // Offset between index banks
#define CALLSIGN_INDEX_BANK_SIZE    0x0001f400  // Size of each callsign index bank

#define CALLSIGN_BANK0              0x04500000  // Start address of the actuall call sign entries
#define CALLSIGN_BANK_OFFSET        0x00040000  // Offset between callsign entry bank
#define CALLSIGN_BANK_SIZE          0x000186a0  // Size of each callsign entry bank

#define CALLSIGN_LIMITS             0x044C0000  // Start address of callsign db limits


/* ******************************************************************************************** *
 * Implementation of D868UVCallsignDB::enty_head_t
 * ******************************************************************************************** */
size_t
D868UVCallsignDB::entry_t::fromUser(const UserDatabase::User &user) {
  call_type = PRIVATE_CALL;
  ring = RING_NONE;
  encode_dmr_id_bcd((uint8_t *)&id, user.id);
  uint8_t size = 6;
  char *strings = (char *)body;
  // append name
  uint n = std::min(16, user.name.size());
  strncpy(strings, user.name.toLocal8Bit().data(), n); size += n; strings += n;
  (*strings) = 0x00; size ++; strings ++;
  // append city
  n = std::min(16, user.city.size());
  strncpy(strings, user.city.toLocal8Bit().data(), n); size += n; strings += n;
  (*strings) = 0x00; size ++; strings ++;
  // append call
  n = std::min(8, user.call.size());
  strncpy(strings, user.call.toLocal8Bit().data(), n); size += n; strings += n;
  (*strings) = 0x00; size ++; strings ++;
  // append state
  n = std::min(16, user.state.size());
  strncpy(strings, user.state.toLocal8Bit().data(), n); size += n; strings += n;
  (*strings) = 0x00; size ++; strings ++;
  // append country
  n = std::min(16, user.country.size());
  strncpy(strings, user.country.toLocal8Bit().data(), n); size += n; strings += n;
  (*strings) = 0x00; size ++; strings ++;
  // no comment
  (*strings) = 0x00; size ++; strings ++;
  return size;
}

size_t
D868UVCallsignDB::entry_t::getSize(const UserDatabase::User &user) {
  return 6 // header
      + std::min(16, user.name.size())+1 // name
      + std::min(16, user.city.size())+1 // city
      + std::min( 8, user.call.size())+1 // call
      + std::min(16, user.state.size())+1 // state
      + std::min(16, user.country.size())+1 // country
      + 1; // no comment but 0x00 terminator
}


/* ******************************************************************************************** *
 * Implementation of D878UVCallsignDB::limits_t
 * ******************************************************************************************** */
void
D868UVCallsignDB::limits_t::clear() {
  count = end_of_db = _unused8 = _unused12 = 0;
}

void
D868UVCallsignDB::limits_t::setCount(uint32_t n) {
  count = qToLittleEndian(n);
}

void
D868UVCallsignDB::limits_t::setTotalSize(uint32_t size) {
  end_of_db = qToLittleEndian(CALLSIGN_BANK0 + size);
}


/* ********************************************************************************************* *
 * Implementation of D878UVCallsignDB
 * ********************************************************************************************* */
D868UVCallsignDB::D868UVCallsignDB(QObject *parent)
  : CallsignDB(parent)
{
  // allocate and clear DB memory
  addImage("AnyTone AT-D878UV Callsign database.");
}

bool D868UVCallsignDB::encode(UserDatabase *db, const Selection &selection) {
  // Determine size of call-sign DB in memory
  qint64 n = std::min(db->count(), qint64(MAX_CALLSIGNS));
  // If DB size is limited by settings
  if (selection.hasCountLimit())
    n = std::min(n, (qint64)selection.countLimit());

  // Select n users and sort them in ascending order of their IDs
  QVector<UserDatabase::User> users;
  users.reserve(n);
  for (uint i=0; i<n; i++)
    users.append(db->user(i));
  std::sort(users.begin(), users.end(),
            [](const UserDatabase::User &a, const UserDatabase::User &b) { return a.id < b.id; });

  // Compute total size of callsign db entries
  size_t dbSize = 0;
  size_t indexSize = n*sizeof(index_entry_t);
  for (qint64 i=0; i<n; i++)
    dbSize += entry_t::getSize(users[i]);

  // Allocate DB limits
  image(0).addElement(CALLSIGN_LIMITS, sizeof(limits_t));
  memset(data(CALLSIGN_LIMITS), 0x00, sizeof(limits_t));
  // Store DB limits
  limits_t *limits = (limits_t *)data(CALLSIGN_LIMITS);
  limits->setCount(n);
  limits->setTotalSize(dbSize);

  // Allocate index banks
  for (int i=0; 0<indexSize; i++, indexSize-=std::min(indexSize, size_t(CALLSIGN_INDEX_BANK_SIZE))) {
    size_t addr = CALLSIGN_INDEX_BANK0 + i*CALLSIGN_INDEX_BANK_OFFSET;
    size_t size = align_size(std::min(indexSize, size_t(CALLSIGN_INDEX_BANK_SIZE)), 16);
    image(0).addElement(addr, size);
    memset(data(addr), 0xff, size);
  }

  // Allocate entry banks
  for (int i=0; 0<dbSize; i++, dbSize-=std::min(dbSize, size_t(CALLSIGN_BANK_SIZE))) {
    size_t addr = CALLSIGN_BANK0 + i*CALLSIGN_BANK_OFFSET;
    size_t size = align_size(std::min(dbSize, size_t(CALLSIGN_BANK_SIZE)), 16);
    image(0).addElement(addr, size);
    memset(data(addr), 0x00, size);
  }

  // Fill index, the offset of the entry is not the real memory offset,
  // but a virtual one without the gaps.
  uint32_t entry_offset = 0;
  uint32_t index_offset = 0;
  uint32_t index_bank   = 0;
  for (qint64 i=0; i<n; i++, index_offset+=sizeof(index_entry_t)) {
    if (CALLSIGN_INDEX_BANK_SIZE <= index_offset) {
      index_offset = 0; index_bank += 1;
    }
    index_entry_t *index = (index_entry_t *)data(
          CALLSIGN_INDEX_BANK0+index_bank*CALLSIGN_BANK_OFFSET+index_offset);
    index->setID(users[i].id, false);
    index->setIndex(entry_offset);
    entry_offset += entry_t::getSize(users[i]);
  }

  // Then store DB entries
  uint32_t entry_bank = 0;
  entry_offset = 0;
  for (qint64 i=0; i<n; i++) {
    // Get size of current entry
    uint32_t entry_size = entry_t::getSize(users[i]);
    // Check if entry fits into bank
    if (CALLSIGN_BANK_SIZE < (entry_offset+entry_size)) {
      // If not, split
      entry_t entry; entry.fromUser(users[i]);
      uint32_t n1 = (CALLSIGN_BANK_SIZE-entry_offset);
      uint32_t n2 = entry_size-n1;
      // Copy first half
      uint8_t *ptr = data(CALLSIGN_BANK0+entry_bank*CALLSIGN_BANK_OFFSET+entry_offset);
      memcpy(ptr, &entry, n1);
      // advance bank counter
      entry_bank++; entry_offset = 0;
      // copy second half
      ptr = data(CALLSIGN_BANK0+entry_bank*CALLSIGN_BANK_OFFSET+entry_offset);
      memcpy(ptr, ((uint8_t *)&entry)+n1, n2);
      // Update offset
      entry_offset += n2;
    } else {
      // when it fits, just add
      entry_t *ptr = (entry_t *)data(CALLSIGN_BANK0+entry_bank*CALLSIGN_BANK_OFFSET+entry_offset);
      ptr->fromUser(users[i]);
      entry_offset += entry_size;
    }
  }

  return true;
}

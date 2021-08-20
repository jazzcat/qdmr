#ifndef MD2017CODEPLUG_HH
#define MD2017CODEPLUG_HH

#include "tyt_codeplug.hh"

/** Device specific implementation of the codeplug for the TyT MD-2017.
 *
 * @ingroup md2017 */
class MD2017Codeplug : public TyTCodeplug
{
  Q_OBJECT

public:
  /** Constructor. */
  explicit MD2017Codeplug(QObject *parent = nullptr);
  /** Destructor. */
  virtual ~MD2017Codeplug();

public:
  void clearChannels();
  bool encodeChannels(Config *config, const Flags &flags);
  bool createChannels(CodeplugContext &ctx);
  bool linkChannels(CodeplugContext &ctx);

  void clearContacts();
  bool encodeContacts(Config *config, const Flags &flags);
  bool createContacts(CodeplugContext &ctx);

  void clearZones();
  bool encodeZones(Config *config, const Flags &flags);
  bool createZones(CodeplugContext &ctx);
  bool linkZones(CodeplugContext &ctx);
};

#endif // MD2017CODEPLUG_HH

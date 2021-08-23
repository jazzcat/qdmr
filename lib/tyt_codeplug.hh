#ifndef TYT_CODEPLUG_HH
#define TYT_CODEPLUG_HH

#include <QDateTime>

#include "codeplug.hh"
#include "signaling.hh"
#include "channel.hh"
#include "contact.hh"

class DigitalContact;
class Zone;
class RXGroupList;
class ScanList;
class GPSSystem;
class CodeplugContext;


/** Base class of all TyT codeplugs. This class implements the majority of all codeplug elements
 * present in all TyT codeplugs. This eases the support of several TyT radios, as only the
 * differences in the codeplug to this base class must be implemented.
 *
 * @ingroup tyt */
class TyTCodeplug : public CodePlug
{
  Q_OBJECT

public:
  /** Represents a single channel (analog or digital) within the TyT codeplug.
   *
   * Memory layout of encoded channel:
   * @verbinclude tytchannel.txt */
  class ChannelElement: public CodePlug::Element
  {
  public:
    /** Possible modes for the channel, i.e. analog and digital. */
    typedef enum {
      MODE_ANALOG  = 1,             ///< Analog channel.
      MODE_DIGITAL = 2              ///< Digital channel.
    } Mode;

    /** Bandwidth of the channel. */
    typedef enum {
      BW_12_5_KHZ = 0,              ///< 12.5 kHz narrow, (default for binary channels).
      BW_20_KHZ   = 1,              ///< 20 kHz (really?)
      BW_25_KHZ   = 2               ///< 25kHz wide.
    } Bandwidth;

    /** Possible privacy types. */
    typedef enum {
      PRIV_NONE = 0,                ///< No privacy.
      PRIV_BASIC = 1,               ///< Basic privacy.
      PRIV_ENHANCED = 2             ///< Enhenced privacy.
    } PrivacyType;

    /** I have absolutely no idea what this means. */
    typedef enum {
      REF_LOW = 0,
      REF_MEDIUM = 1,
      REF_HIGH = 2
    } RefFrequency;

    /** TX Admit criterion. */
    typedef enum {
      ADMIT_ALWAYS = 0,             ///< Always allow TX.
      ADMIT_CH_FREE = 1,            ///< Allow TX if channel is free.
      ADMIT_TONE = 2,               ///< Allow TX if CTCSS tone matches.
      ADMIT_COLOR = 3,              ///< Allow TX if color-code matches.
    } Admit;

    /** Again, I have no idea. */
    typedef enum {
      INCALL_ALWAYS = 0,
      INCALL_ADMIT = 1,
      INCALL_TXINT = 2
    } InCall;

    /** Turn-off tone frequency.
     * This radio has a feature that allows to disable radios remotely by sending a specific tone.
     * Certainly not a feature used in ham-radio. */
    typedef enum {
      TURNOFF_NONE = 3,             ///< Turn-off disabled. Default!
      TURNOFF_259_2HZ = 0,          ///< Turn-off on 259.2Hz tone.
      TURNOFF_55_2HZ = 1            ///< Turn-off on 55.2Hz tone.
    } TurnOffFreq;


  protected:
    /** Constructs a channel from the given memory. */
    ChannelElement(uint8_t *ptr, size_t size);

  public:
    /** Constructs a channel from the given memory. */
    ChannelElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ChannelElement();

    /** Returns @c true if channel is valid/enabled. */
    bool isValid() const;
    /** Clears/resets the channel and therefore disables it. */
    void clear();

    /** Returns the mode of the channel. */
    virtual Mode mode() const;
    /** Sets the mode of the channel. */
    virtual void mode(Mode mode);

    /** Retuns the bandwidth of the (analog) channel. */
    virtual AnalogChannel::Bandwidth bandwidth() const;
    /** Sets the bandwidth of the (analog) channel. */
    virtual void bandwidth(AnalogChannel::Bandwidth bw);

    /** Returns @c true if the channel has auto scan enabled. */
    virtual bool autoScan() const;
    /** Enables/disables auto scan for this channel. */
    virtual void autoScan(bool enable);

    /** Returns @c true if the channel has lone worker enabled. */
    virtual bool loneWorker() const;
    /** Enables/disables lone worker for this channel. */
    virtual void loneWorker(bool enable);

    /** Returns @c true if the channel has talkaround enabled. */
    virtual bool talkaround() const;
    /** Enables/disables talkaround for this channel. */
    virtual void talkaround(bool enable);

    /** Returns @c true if the channel has rx only enabled. */
    virtual bool rxOnly() const;
    /** Enables/disables rx only for this channel. */
    virtual void rxOnly(bool enable);

    /** Returns the time slot of this channel. */
    virtual DigitalChannel::TimeSlot timeSlot() const;
    /** Sets the time slot of this channel. */
    virtual void timeSlot(DigitalChannel::TimeSlot ts);

    /** Returns the color code of this channel. */
    virtual uint8_t colorCode() const;
    /** Sets the color code of this channel. */
    virtual void colorCode(uint8_t ts);

    /** Returns the index (+1) of the privacy system (key). */
    virtual uint8_t privacyIndex() const;
    /** Sets the index (+1) of the privacy system (key). */
    virtual void privacyIndex(uint8_t ts);
    /** Returns the type of the privacy system. */
    virtual PrivacyType privacyType() const;
    /** Sets the type of the privacy system. */
    virtual void privacyType(PrivacyType type);

    /** Returns @c true if the channel has private call confirmation enabled. */
    virtual bool privateCallConfirm() const;
    /** Enables/disables private call confirmation for this channel. */
    virtual void privateCallConfirm(bool enable);

    /** Returns @c true if the channel has data call confirmation enabled. */
    virtual bool dataCallConfirm() const;
    /** Enables/disables data call confirmation for this channel. */
    virtual void dataCallConfirm(bool enable);

    /** Returns some weird reference frequency setting for reception. */
    virtual RefFrequency rxRefFrequency() const;
    /** Sets some weird reference frequency setting for reception. */
    virtual void rxRefFrequency(RefFrequency ref);
    /** Returns some weird reference frequency setting for transmission. */
    virtual RefFrequency txRefFrequency() const;
    /** Sets some weird reference frequency setting for transmission. */
    virtual void txRefFrequency(RefFrequency ref);

    /** Returns @c true if the channel has alarm confirmation enabled. */
    virtual bool emergencyAlarmACK() const;
    /** Enables/disables alarm confirmation for this channel. */
    virtual void emergencyAlarmACK(bool enable);

    /** Returns @c true if the channel has display PTT ID enabled. */
    virtual bool displayPTTId() const;
    /** Enables/disables PTT ID display for this channel. */
    virtual void displayPTTId(bool enable);

    /** Returns @c true if the channel has VOX enabled. */
    virtual bool vox() const;
    /** Enables/disables VOX for this channel. */
    virtual void vox(bool enable);

    /** Returns the admit criterion for this channel. */
    virtual Admit admitCriterion() const;
    /** Sets the admit criterion for this channel. */
    virtual void admitCriterion(Admit admit);

    /** Returns the in-call criterion for this channel. */
    virtual InCall inCallCriteria() const;
    /** Sets the in-call criterion for this channel. */
    virtual void inCallCriteria(InCall crit);

    /** Returns the remote turn-off/kill frequency for this channel. */
    virtual TurnOffFreq turnOffFreq() const;
    /** Sets the remote turn-off/kill frequency for this channel. */
    virtual void turnOffFreq(TurnOffFreq freq);

    /** Returns the transmit contact index (+1) for this channel. */
    virtual uint16_t contactIndex() const;
    /** Sets the transmit contact index (+1) for this channel. */
    virtual void contactIndex(uint16_t idx);

    /** Returns the transmit time-out in seconds. */
    virtual uint txTimeOut() const;
    /** Sets the transmit time-out in seconds. */
    virtual void txTimeOut(uint tot);
    /** Returns the transmit time-out re-key delay in seconds. */
    virtual uint8_t txTimeOutRekeyDelay() const;
    /** Sets the transmit time-out re-key delay in seconds. */
    virtual void txTimeOutRekeyDelay(uint8_t delay);

    /** Returns the emergency system index (+1) for this channel. */
    virtual uint8_t emergencySystemIndex() const;
    /** Sets the emergency system index (+1) for this channel. */
    virtual void emergencySystemIndex(uint8_t idx);

    /** Returns the scan-list index (+1) for this channel. */
    virtual uint8_t scanListIndex() const;
    /** Sets the scan-list index (+1) for this channel. */
    virtual void scanListIndex(uint8_t idx);

    /** Returns the RX group list index (+1) for this channel. */
    virtual uint8_t groupListIndex() const;
    /** Sets the RX group list index (+1) for this channel. */
    virtual void groupListIndex(uint8_t idx);

    /** Returns the positioning system index (+1) for this channel. */
    virtual uint8_t positioningSystemIndex() const;
    /** Sets the positioning system index (+1) for this channel. */
    virtual void positioningSystemIndex(uint8_t idx);

    /** Returns @c true if the channel has DTMF decoding enabled. */
    virtual bool dtmfDecode(uint8_t idx) const;
    /** Enables/disables DTMF decoding this channel. */
    virtual void dtmfDecode(uint8_t idx, bool enable);

    /** Returns the squelch level [0-10]. */
    virtual uint squelch() const;
    /** Sets the squelch level [0-10]. */
    virtual void squelch(uint value);

    /** Returns the RX frequency in Hz. */
    virtual uint32_t rxFrequency() const;
    /** Sets the RX frequency in Hz. */
    virtual void rxFrequency(uint32_t Hz);
    /** Returns the TX frequency in Hz. */
    virtual uint32_t txFrequency() const;
    /** Sets the TX frequency in Hz. */
    virtual void txFrequency(uint32_t Hz);

    /** Returns the CTCSS/DSC signaling for RX. */
    virtual Signaling::Code rxSignaling() const;
    /** Sets the CTCSS/DSC signaling for RX. */
    virtual void rxSignaling(Signaling::Code code);
    /** Returns the CTCSS/DSC signaling for TX. */
    virtual Signaling::Code txSignaling() const;
    /** Sets the CTCSS/DSC signaling for TX. */
    virtual void txSignaling(Signaling::Code code);
    /** Returns the signaling system index (+1) for RX. */
    virtual uint8_t rxSignalingSystemIndex() const;
    /** Sets the signaling system index (+1) for RX. */
    virtual void rxSignalingSystemIndex(uint8_t idx);
    /** Returns the signaling system index (+1) for TX. */
    virtual uint8_t txSignalingSystemIndex() const;
    /** Sets the signaling system index (+1) for TX. */
    virtual void txSignalingSystemIndex(uint8_t idx);

    /** Returns the power of this channel. */
    virtual Channel::Power power() const;
    /** Sets the power of this channel. */
    virtual void power(Channel::Power pwr);

    /** Returns @c true if the channel transmits GPS information enabled. */
    virtual bool txGPSInfo() const;
    /** Enables/disables transmission of GPS information for this channel. */
    virtual void txGPSInfo(bool enable);
    /** Returns @c true if the channel receives GPS information enabled. */
    virtual bool rxGPSInfo() const;
    /** Enables/disables reception of GPS information for this channel. */
    virtual void rxGPSInfo(bool enable);

    /** Returns @c true if the channel allows interruption enabled. */
    virtual bool allowInterrupt() const;
    /** Enables/disables interruption for this channel. */
    virtual void allowInterrupt(bool enable);

    /** Returns @c true if the channel has dual-capacity direct mode enabled. */
    virtual bool dualCapacityDirectMode() const;
    /** Enables/disables dual-capacity direct mode for this channel. */
    virtual void dualCapacityDirectMode(bool enable);

    /** Retruns @c true if the radio acts as the leader for this DCDM channel. */
    virtual bool leaderOrMS() const;
    /** Enables/disables this radio to be the leader for this DCDM channel. */
    virtual void leaderOrMS(bool enable);

    /** Returns the name of this channel. */
    virtual QString name() const;
    /** Sets the name of this channel. */
    virtual void name(const QString &name);

    /** Constructs a generic @c Channel object from the codeplug channel. */
    virtual Channel *toChannelObj() const;
    /** Links a previously constructed channel to the rest of the configuration. */
    virtual bool linkChannelObj(Channel *c, const CodeplugContext &ctx) const;
    /** Initializes this codeplug channel from the given generic configuration. */
    virtual void fromChannelObj(const Channel *c, const CodeplugContext &ctx);
  };

  /** Implements a VFO channel for TyT radios.
   * This class is an extension of the normal ChannelElement that only implements the step-size
   * feature and encodes it where the name used to be. Thus the memory layout and size is identical
   * to the normal channel. */
  class VFOChannelElement: public ChannelElement
  {
  protected:
    /** Constructor from pointer to memory. */
    VFOChannelElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor from pointer to memory. */
    VFOChannelElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~VFOChannelElement();

    QString name() const;
    void name(const QString &txt);

    /** Returns the step-size for the VFO channel. */
    virtual uint stepSize() const;
    /** Sets the step-size for the VFO channel in Hz. */
    virtual void stepSize(uint ss_hz);
  };

  /** Represents a digital (DMR) contact within the codeplug.
   *
   * Memory layout of encoded contact:
   * @verbinclude tytcontact.txt */
  class ContactElement: public CodePlug::Element
  {
  protected:
    /** Constructor. */
    ContactElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    ContactElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ContactElement();

    void clear();
    bool isValid() const;

    /** Returns the DMR ID of the contact. */
    virtual uint32_t dmrId() const;
    /** Sets the DMR ID of the contact. */
    virtual void dmrId(uint32_t id);

    /** Returns the call-type of the contact. */
    virtual DigitalContact::Type callType() const;
    /** Sets the call-type of the contact. */
    virtual void callType(DigitalContact::Type type);

    /** Returns @c true if the ring-tone is enabled for this contact. */
    virtual bool ringTone() const;
    /** Enables/disables the ring-tone for this contact. */
    virtual void ringTone(bool enable);

    /** Returns the name of the contact. */
    virtual QString name() const;
    /** Sets the name of the contact. */
    virtual void name(const QString &nm);

    /** Encodes the give contact. */
    virtual bool fromContactObj(const DigitalContact *contact);
    /** Creates a contact. */
    virtual DigitalContact *toContactObj() const;
  };

  /** Represents a zone within the codeplug.
   * Please note that a zone consists of two elements the @c ZoneElement and the @c ZoneExtElement.
   * The latter adds additional channels for VFO A and the channels for VFO B.
   *
   * Memory layout of encoded zone:
   * @verbinclude tytzone.txt */
  class ZoneElement: public CodePlug::Element
  {
  protected:
    /** Construtor. */
    ZoneElement(uint8_t *ptr, size_t size);

  public:
    /** Construtor. */
    ZoneElement(uint8_t *ptr);
    /** Desturctor. */
    virtual ~ZoneElement();

    void clear();
    bool isValid() const;

    /** Returns the name of the zone. */
    virtual QString name() const;
    /** Sets the name of the zone. */
    virtual void name(const QString &name);

    /** Returns the index (+1) of the @c n-th member. */
    virtual uint16_t memberIndex(uint n) const;
    /** Sets the index (+1) of the @c n-th member. */
    virtual void memberIndex(uint n, uint16_t idx);

    /** Encodes a given zone object. */
    virtual bool fromZoneObj(const Zone *zone, const CodeplugContext &ctx);
    /** Creates a zone. */
    virtual Zone *toZoneObj() const;
    /** Links the created zone to channels. */
    virtual bool linkZone(Zone *zone, const CodeplugContext &ctx) const;
  };

  /** Extended zone data.
   * The zone definition @c ZoneElement contains only a single set of 16 channels. For each zone
   * definition, there is a zone extension which extends a zone to zwo sets of 64 channels each.
   *
   * Memory layout of encoded zone extension:
   * @verbinclude tytzoneext.txt */
  class ZoneExtElement: public CodePlug::Element
  {
  protected:
    /** Constructor. */
    ZoneExtElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    ZoneExtElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ZoneExtElement();

    void clear();

    /** Returns the n-th member index of the channel list for A. */
    virtual uint16_t memberIndexA(uint n) const;
    /** Sets the n-th member index of the channel list for A. */
    virtual void memberIndexA(uint n, uint16_t idx);
    /** Returns the n-th member index of the channel list for B. */
    virtual uint16_t memberIndexB(uint n) const;
    /** Returns the n-th member index of the channel list for B. */
    virtual void memberIndexB(uint n, uint16_t idx);

    /** Encodes the given zone. */
    virtual bool fromZoneObj(const Zone *zone, const CodeplugContext &ctx);
    /** Links the given zone object.
     * Thant is, extends channel list A and populates channel list B. */
    virtual bool linkZoneObj(Zone *zone, const CodeplugContext &ctx);
  };

  /** Representation of an RX group list within the codeplug.
   *
   * Memory layout of encoded RX group list:
   * @verbinclude tytgrouplist.txt */
  class GroupListElement: public CodePlug::Element
  {
  protected:
    /** Constructor. */
    GroupListElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    GroupListElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~GroupListElement();

    void clear();
    bool isValid() const;

    /** Returns the name of the group list. */
    virtual QString name() const;
    /** Sets the name of the group list. */
    virtual void name(const QString &nm);

    /** Returns the n-th member index. */
    virtual uint16_t memberIndex(uint n) const;
    /** Sets the n-th member index. */
    virtual void memberIndex(uint n, uint16_t idx);

    /** Encodes the given group list. */
    virtual bool fromGroupListObj(const RXGroupList *lst, const CodeplugContext &ctx);
    /** Creates a group list object. */
    virtual RXGroupList *toGroupListObj(const CodeplugContext &ctx);
    /** Links the given group list. */
    virtual bool linkGroupListObj(RXGroupList *lst, const CodeplugContext &ctx);
  };

  /** Represents a scan list within the codeplug.
   *
   * Memory layout of encoded scan list:
   * @verbinclude tytscanlist.txt */
  class ScanListElement: public CodePlug::Element
  {
  protected:
    /** Constructor. */
    ScanListElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    ScanListElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ScanListElement();

    bool isValid() const;
    void clear();

    /** Retruns the name of the scan list. */
    virtual QString name() const;
    /** Sets the name of the scan list. */
    virtual void name(const QString &nm);

    /** Returns the index (+1) of the first priority channel. */
    virtual uint16_t priorityChannel1Index() const;
    /** Set the index (+1) of the first priority channel. */
    virtual void priorityChannel1Index(uint16_t idx);

    /** Returns the index (+1) of the second priority channel. */
    virtual uint16_t priorityChannel2Index() const;
    /** Set the index (+1) of the second priority channel. */
    virtual void priorityChannel2Index(uint16_t idx);

    /** Returns the index (+1) of the TX channel. 0=current, 0xffff=none. */
    virtual uint16_t txChannelIndex() const;
    /** Sets the index (+1) of the TX channel. 0=current, 0xffff=none. */
    virtual void txChannelIndex(uint16_t idx);

    /** Returns the hold time in ms. */
    virtual uint holdTime() const;
    /** Sets the hold time in ms. */
    virtual void holdTime(uint time);

    /** Returns the priority sample time in ms. */
    virtual uint prioritySampleTime() const;
    /** Sets the priority sample time in ms. */
    virtual void prioritySampleTime(uint time);

    /** Returns the n-th member index. */
    virtual uint16_t memberIndex(uint n) const;
    /** Sets the n-th member index. */
    virtual void memberIndex(uint n, uint16_t idx);

    /** Encodes the given scan list. */
    virtual bool fromScanListObj(const ScanList *lst, const CodeplugContext &ctx);
    /** Creates a scan list. */
    virtual ScanList *toScanListObj(const CodeplugContext &ctx);
    /** Links the scan list object. */
    virtual bool linkScanListObj(ScanList *lst, const CodeplugContext &ctx);
  };

  /** Codeplug representation of the general settings.
   *
   * Memory layout of encoded settings:
   * @verbinclude tytsettings.txt */
  class GeneralSettingsElement: public CodePlug::Element
  {
  public:
    /** Defines all possible transmit modes. */
    enum TransmitMode {
      LAST_CALL_CH = 0,
      LAST_CALL_AND_HAND_CH = 1,
      DESIGNED_CH = 2,
      DESIGNED_AND_HAND_CH = 3,
    };

    /** Defines all possible squelch modes. */
    enum MonitorType {
      MONITOR_SILENT       = 0,
      MONITOR_OPEN_SQUELCH = 1
    };

  protected:
    /** Hidden constructor. */
    GeneralSettingsElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit GeneralSettingsElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~GeneralSettingsElement();

    void clear();

    /** Returns the first intro line. */
    virtual QString introLine1() const;
    /** Sets the first intro line. */
    virtual void introLine1(const QString line);
    /** Returns the second intro line. */
    virtual QString introLine2() const;
    /** Sets the second intro line. */
    virtual void introLine2(const QString line);

    /** Returns the transmit mode. */
    virtual TransmitMode transmitMode() const;
    /** Sets the transmit mode. */
    virtual void transmitMode(TransmitMode mode);

    /** Returns the monitor type. */
    virtual MonitorType monitorType() const;
    /** Sets the monitor type. */
    virtual void monitorType(MonitorType type);

    /** Returns @c true if all LEDs are disabled. */
    virtual bool allLEDsDisabled() const;
    /** Enables/disables all LEDs. */
    virtual void disableAllLEDs(bool disable);

    /** Returns @c true, if save preamble is enabled. */
    virtual bool savePreamble() const;
    /** Enables/disables save preamble. */
    virtual void savePreamble(bool enable);
    /** Returns @c true, if save RX mode is enabled. */
    virtual bool saveModeRX() const;
    /** Enables/disables save mode RX. */
    virtual void saveModeRX(bool enable);
    /** Returns @c true, if all tones are disabled. */
    virtual bool allTonesDisabled() const;
    /** Enables/disables all tones. */
    virtual void disableAllTones(bool disable);
    /** Returns @c true, if the channel free indication tone is enabled. */
    virtual bool chFreeIndicationTone() const;
    /** Enables/disables the channel free indication tone. */
    virtual void chFreeIndicationTone(bool enable);
    /** Returns @c true, if password and lock is enabled. */
    virtual bool passwdAndLock() const;
    /** Enables/disables password and lock. */
    virtual void passwdAndLock(bool enable);
    /** Returns @c true, if the talk permit tone is enabled for DMR channels. */
    virtual bool talkPermitToneDigital() const;
    /** Enables/disables talk permit tone for DMR channels. */
    virtual void talkPermitToneDigital(bool enable);
    /** Returns @c true, if the talk permit tone is enabled for analog channels. */
    virtual bool talkPermitToneAnalog() const;
    /** Enables/disables talk permit tone for analog channels. */
    virtual void talkPermitToneAnalog(bool enable);

    /** Returns @c true, if the speech synthesis is enabled. */
    virtual bool channelVoiceAnnounce() const;
    /** Enables/disables the speech synthesis. */
    virtual void channelVoiceAnnounce(bool enable);
    /** Returns @c true, if intro picture is enabled. */
    virtual bool introPicture() const;
    /** Enables/disables the intro picture. */
    virtual void introPicture(bool enable);
    /** Returns @c true, if keypad tones are enabled. */
    virtual bool keypadTones() const;
    /** Enables/disables the keypad tones. */
    virtual void keypadTones(bool enable);

    /** Returns @c true, if VFO A is in channel mode. */
    virtual bool channelModeA() const;
    /** Enables/disables the channel mode for VFO A. */
    virtual void channelModeA(bool enable);
    /** Returns @c true, if VFO B is in channel mode. */
    virtual bool channelModeB() const;
    /** Enables/disables the channel mode for VFO B. */
    virtual void channelModeB(bool enable);

    /** Returns the default DMR ID of the radio. */
    virtual uint32_t dmrID() const;
    /** Sets the default DMR ID of the radio. */
    virtual void dmrID(uint32_t id);

    /** Returns the TX preamble duration. */
    virtual uint txPreambleDuration() const;
    /** Sets the TX preamble duration. */
    virtual void txPreambleDuration(uint ms);
    /** Returns the group call hang time. */
    virtual uint groupCallHangTime() const;
    /** Sets the group call hang time. */
    virtual void groupCallHangTime(uint ms);
    /** Returns the private call hang time. */
    virtual uint privateCallHangTime() const;
    /** Sets the private call hang time. */
    virtual void privateCallHangTime(uint ms);
    /** Returns the VOX sensitivity. */
    virtual uint voxSesitivity() const;
    /** Sets the group call hang time. */
    virtual void voxSesitivity(uint ms);
    /** Returns the low-battery warning interval. */
    virtual uint lowBatteryInterval() const;
    /** Sets the low-battery warning interval. */
    virtual void lowBatteryInterval(uint sec);

    /** Returns @c true if the call-alert is continuous. */
    virtual bool callAlertToneIsContinuous() const;
    /** Returns the call-alert tone duration. */
    virtual uint callAlertToneDuration() const;
    /** Sets the call-alert tone duration. */
    virtual void callAlertToneDuration(uint sec);
    /** Sets the call-alert tone continuous. */
    virtual void setCallAlertToneContinuous();

    /** Returns the lone-worker response time. */
    virtual uint loneWorkerResponseTime() const;
    /** Sets the lone-worker response time. */
    virtual void loneWorkerResponseTime(uint min);
    /** Returns the lone-worker reminder time. */
    virtual uint loneWorkerReminderTime() const;
    /** Sets the lone-worker reminder time. */
    virtual void loneWorkerReminderTime(uint min);
    /** Returns the scan digital hang time. */
    virtual uint scanDigitalHangTime() const;
    /** Sets the scan digital hang time. */
    virtual void scanDigitalHangTime(uint ms);
    /** Returns the scan analog hang time. */
    virtual uint scanAnalogHangTime() const;
    /** Sets the scan analog hang time. */
    virtual void scanAnalogHangTime(uint ms);

    /** Retuns @c true if the backlight is always on. */
    virtual bool backlightIsAlways() const;
    /** Returns the backlight time. */
    virtual uint backlightTime() const;
    /** Sets the backlight time. */
    virtual void backlightTime(uint sec);
    /** Turns the backlight always on. */
    virtual void backlightTimeSetAlways();

    /** Retuns @c true if the keypad lock is manual. */
    virtual bool keypadLockIsManual() const;
    /** Returns the keypad lock time. */
    virtual uint keypadLockTime() const;
    /** Sets the keypad lock time. */
    virtual void keypadLockTime(uint sec);
    /** Set keypad lock to manual. */
    virtual void keypadLockTimeSetManual();

    /** Returns @c true, if the radio is in channel (and not VFO) mode. */
    virtual bool channelMode() const;
    /** Enable/disable channel mode. */
    virtual void channelMode(bool enable);

    /** Returns the 8-digit power-on password. */
    virtual uint32_t powerOnPassword() const;
    /** Sets the 8-digit power-on password. */
    virtual void powerOnPassword(uint32_t passwd);

    /** Returns @c true, if the radio programming password is enabled. */
    virtual bool radioProgPasswordEnabled() const;
    /** Returns the 8-digit radio programming password. */
    virtual uint32_t radioProgPassword() const;
    /** Sets the 8-digit radio programming password. */
    virtual void radioProgPassword(uint32_t passwd);
    /** Disables the radio programming password. */
    virtual void radioProgPasswordDisable();

    /** Returns @c true, if the PC programming password is enabled. */
    virtual bool pcProgPasswordEnabled() const;
    /** Returns the PC programming password. */
    virtual QString pcProgPassword() const;
    /** Sets the PC programming password. */
    virtual void pcProgPassword(const QString &pass);
    /** Disables the PC programming password. */
    virtual void pcProgPasswordDisable();

    /** Returns @c true if group-call match is enabled. */
    virtual bool groupCallMatch() const;
    /** Enables/disables group-call match. */
    virtual void groupCallMatch(bool enable);
    /** Returns @c true if private-call match is enabled. */
    virtual bool privateCallMatch() const;
    /** Enables/disables private-call match. */
    virtual void privateCallMatch(bool enable);

    /** Returns the time-zone. */
    virtual QTimeZone timeZone() const;
    /** Sets the time-zone. */
    virtual void timeZone(const QTimeZone &zone);

    /** Returns the radio name. */
    virtual QString radioName() const;
    /** Sets the radio name. */
    virtual void radioName(const QString &name);

    /** Returns the channel hang time. */
    virtual uint channelHangTime() const;
    /** Sets the channel hang time. */
    virtual void channelHangTime(uint dur);
    /** Returns @c true, if public zone is enabled. */
    virtual bool publicZone() const;
    /** Enables/disables public zone. */
    virtual void publicZone(bool enable);

    /** Returns the n-th DMR id. */
    virtual uint32_t additionalDMRId(uint n) const;
    /** Sets the n-th DMR id. */
    virtual void additionalDMRId(uint n, uint32_t id);

    /** Returns the microphone gain. */
    virtual uint micLevel() const;
    /** Sets the microphone gain. */
    virtual void micLevel(uint val);

    /** If @c true, radio ID editing is enabled. */
    virtual bool editRadioID() const;
    /** Enable/disable radio ID editing. */
    virtual void editRadioID(bool enable);

    /** Encodes the general settings. */
    virtual bool fromConfig(const Config *config);
    /** Updates config from general settings. */
    virtual bool updateConfig(Config *config);
  };

  /** Represents the boot-time settings (selected zone and channels) within the code-plug.
   *
   * Memory layout of encoded boot settings:
   * @verbinclude tytbootsettings.txt */
  class BootSettingsElement: public CodePlug::Element
  {
  protected:
    /** Hidden constructor. */
    BootSettingsElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit BootSettingsElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~BootSettingsElement();

    void clear();

    /** Returns the boot zone index. */
    virtual uint zoneIndex() const;
    /** Sets the boot zone index. */
    virtual void zoneIndex(uint idx);
    /** Returns the channel index (within zone) for VFO A. */
    virtual uint channelIndexA() const;
    /** Sets the channel index (within zone) for VFO A. */
    virtual void channelIndexA(uint idx);
    /** Returns the channel index (within zone) for VFO B. */
    virtual uint channelIndexB() const;
    /** Sets the channel index (within zone) for VFO B. */
    virtual void channelIndexB(uint idx);
  };

  /** Codeplug representation of programming time-stamp and CPS version.
   *
   * Memory layout of encoded timestamp:
   * @verbinclude tyttimestamp.txt */
  class TimestampElement: public CodePlug::Element
  {
  protected:
    /** Hidden constructor. */
    TimestampElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit TimestampElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~TimestampElement();

    void clear();

    /** Returns the time stamp. */
    virtual QDateTime timestamp() const;
    /** Sets the time stamp. */
    virtual void timestamp(const QDateTime &ts);

    /** Returns the CPS version. */
    virtual QString cpsVersion() const;
  };

  /** Represents a single GPS system within the codeplug.
   *
   * Memory layout of encoded GPS system:
   * @verbinclude tytgpssystem.txt */
  class GPSSystemElement: public CodePlug::Element
  {
  protected:
    /** Hidden constructor. */
    GPSSystemElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit GPSSystemElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~GPSSystemElement();

    bool isValid() const;
    void clear();

    /** Returns @c true if the revert channel is the current one. */
    virtual bool revertChannelIsSelected() const;
    /** Returns the revert channel index (+1). */
    virtual uint16_t revertChannelIndex() const;
    /** Sets the revert channel index (+1). */
    virtual void revertChannelIndex(uint16_t idx);
    /** Sets the revert channel to the current one. */
    virtual void setRevertChannelSelected();

    /** Returns @c true if the repeat interval is disabled. */
    virtual bool repeatIntervalDisabled() const;
    /** Returns the repeat interval. */
    virtual uint repeatInterval() const;
    /** Sets the repeat interval in seconds. */
    virtual void repeatInterval(uint sec);
    /** Disables the GPS repeat interval. */
    virtual void disableRepeatInterval();

    /** Returns @c true if the destination contact is disabled. */
    virtual bool destinationContactDisabled() const;
    /** Returns the destination contact index (+1). */
    virtual uint16_t destinationContactIndex() const;
    /** Sets the destination contact index (+1). */
    virtual void destinationContactIndex(uint16_t idx);
    /** Disables the destination contact. */
    virtual void disableDestinationContact();

    /** Encodes the given GPS system. */
    virtual bool fromGPSSystemObj(const GPSSystem *sys, const CodeplugContext &ctx);
    /** Constructs a GPS system. */
    virtual GPSSystem *toGPSSystemObj();
    /** Links the given GPS system. */
    virtual bool linkGPSSystemObj(GPSSystem *sys, const CodeplugContext &ctx);
  };

  /** Represents all menu settings within the codeplug on the radio.
   *
   * Memory representaion of the menu settings:
   * @verbinclude tytmenusettings.txt */
  class MenuSettingsElement: public CodePlug::Element
  {
  protected:
    /** Hidden constructor. */
    MenuSettingsElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit MenuSettingsElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~MenuSettingsElement();

    void clear();

    /** Returns @c true if the menu hang time is infinite. */
    virtual bool menuHangtimeIsInfinite() const;
    /** Returns the menu hang time in seconds. */
    virtual uint menuHangtime() const;
    /** Sets the menu hang time in seconds. */
    virtual void menuHangtime(uint sec);
    /** Sets the menu hang time to be infinite. */
    virtual void infiniteMenuHangtime();

    /** Returns @c true if text message menu is enabled. */
    virtual bool textMessage() const;
    /** Enables/disables text message menu. */
    virtual void textMessage(bool enable);
    /** Returns @c true if call alert menu is enabled. */
    virtual bool callAlert() const;
    /** Enables/disables call alert menu. */
    virtual void callAlert(bool enable);
    /** Returns @c true if contact editing is enabled. */
    virtual bool contactEditing() const;
    /** Enables/disables contact editing. */
    virtual void contactEditing(bool enable);
    /** Returns @c true if manual dial is enabled. */
    virtual bool manualDial() const;
    /** Enables/disables manual dial. */
    virtual void manualDial(bool enable);
    /** Returns @c true if contact radio-check menu is enabled. */
    virtual bool contactRadioCheck() const;
    /** Enables/disables contact radio-check menu. */
    virtual void contactRadioCheck(bool enable);
    /** Returns @c true if remote monitor menu is enabled. */
    virtual bool remoteMonitor() const;
    /** Enables/disables remote monitor menu. */
    virtual void remoteMonitor(bool enable);
    /** Returns @c true if radio enable menu is enabled. */
    virtual bool radioEnable() const;
    /** Enables/disables radio enable menu. */
    virtual void radioEnable(bool enable);
    /** Returns @c true if radio disable menu is enabled. */
    virtual bool radioDisable() const;
    /** Enables/disables radio disable menu. */
    virtual void radioDisable(bool enable);

    /** Returns @c true if scan menu is enabled. */
    virtual bool scan() const;
    /** Enables/disables scan menu. */
    virtual void scan(bool enable);
    /** Returns @c true if edit scan-list menu is enabled. */
    virtual bool editScanlist() const;
    /** Enables/disables edit scan-list menu. */
    virtual void editScanlist(bool enable);
    /** Returns @c true if call-log missed menu is enabled. */
    virtual bool callLogMissed() const;
    /** Enables/disables call-log missed menu. */
    virtual void callLogMissed(bool enable);
    /** Returns @c true if call-log answered menu is enabled. */
    virtual bool callLogAnswered() const;
    /** Enables/disables call-log answered menu. */
    virtual void callLogAnswered(bool enable);
    /** Returns @c true if call-log outgoing menu is enabled. */
    virtual bool callLogOutgoing() const;
    /** Enables/disables call-log outgoing menu. */
    virtual void callLogOutgoing(bool enable);
    /** Returns @c true if talkaround menu is enabled. */
    virtual bool talkaround() const;
    /** Enables/disables talkaround menu. */
    virtual void talkaround(bool enable);
    /** Returns @c true if tone/alert menu is enabled. */
    virtual bool toneAlert() const;
    /** Enables/disables tone/alert  menu. */
    virtual void toneAlert(bool enable);

    /** Returns @c true if power menu is enabled. */
    virtual bool power() const;
    /** Enables/disables power menu. */
    virtual void power(bool enable);
    /** Returns @c true if backlight menu is enabled. */
    virtual bool backlight() const;
    /** Enables/disables backlight menu. */
    virtual void backlight(bool enable);
    /** Returns @c true if intro screen menu is enabled. */
    virtual bool introScreen() const;
    /** Enables/disables intro screen menu. */
    virtual void introScreen(bool enable);
    /** Returns @c true if keypad lock menu is enabled. */
    virtual bool keypadLock() const;
    /** Enables/disables keypad lock menu. */
    virtual void keypadLock(bool enable);
    /** Returns @c true if LED indicator menu is enabled. */
    virtual bool ledIndicator() const;
    /** Enables/disables LED indicator menu. */
    virtual void ledIndicator(bool enable);
    /** Returns @c true if squelch menu is enabled. */
    virtual bool squelch() const;
    /** Enables/disables squelch menu. */
    virtual void squelch(bool enable);
    /** Returns @c true if VOX menu is enabled. */
    virtual bool vox() const;
    /** Enables/disables VOX menu. */
    virtual void vox(bool enable);

    /** Returns @c true if password menu is enabled. */
    virtual bool password() const;
    /** Enables/disables password menu. */
    virtual void password(bool enable);
    /** Returns @c true if display mode menu is enabled. */
    virtual bool displayMode() const;
    /** Enables/disables display mode menu. */
    virtual void displayMode(bool enable);
    /** Returns @c true if program radio menu is enabled. */
    virtual bool programRadio() const;
    /** Enables/disables program radio menu. */
    virtual void programRadio(bool enable);
    /** Returns @c true if GPS settings menu is enabled. */
    virtual bool gpsSettings() const;
    /** Enables/disables GPS settings menu. */
    virtual void gpsSettings(bool enable);
    /** Returns @c true if GPS information is enabled. */
    virtual bool gpsInformation() const;
    /** Enables/disables GPS information menu. */
    virtual void gpsInformation(bool enable);
    /** Returns @c true if recording menu is enabled. */
    virtual bool recording() const;
    /** Enables/disables recording menu. */
    virtual void recording(bool enable);

    /** Returns @c true if group call match menu is enabled. */
    virtual bool groupCallMatch() const;
    /** Enables/disables group call match menu. */
    virtual void groupCallMatch(bool enable);
    /** Returns @c true if private call match menu is enabled. */
    virtual bool privateCallMatch() const;
    /** Enables/disables private call match menu. */
    virtual void privateCallMatch(bool enable);
    /** Returns @c true if menu hang time item is enabled. */
    virtual bool menuHangtimeItem() const;
    /** Enables/disables menu hang time item. */
    virtual void menuHangtimeItem(bool enable);
    /** Returns @c true if TX mode menu is enabled. */
    virtual bool txMode() const;
    /** Enables/disables TX mode menu. */
    virtual void txMode(bool enable);
    /** Returns @c true if zone settings menu is enabled. */
    virtual bool zoneSettings() const;
    /** Enables/disables zone settings menu. */
    virtual void zoneSettings(bool enable);
    /** Returns @c true if new zone menu is enabled. */
    virtual bool newZone() const;
    /** Enables/disables new zone menu. */
    virtual void newZone(bool enable);

    /** Returns @c true if edit zone menu is enabled. */
    virtual bool editZone() const;
    /** Enables/disables edit zone menu. */
    virtual void editZone(bool enable);
    /** Returns @c true if new scan list menu is enabled. */
    virtual bool newScanList() const;
    /** Enables/disables new scan list menu. */
    virtual void newScanList(bool enable);
  };

  /** Represents all button settings within the codeplug on the radio.
   *
   * Memory representation of the button settings:
   * @verbinclude tytbuttonsettings.txt */
  class ButtonSettingsElement: public CodePlug::Element
  {
  public:
    /** Possible actions for the side-buttons. */
    enum ButtonAction {
      Disabled = 0,                       ///< Disabled side-button action.
      ToggleAllAlertTones = 1,            ///< Toggle all alert tones.
      EmergencyOn = 2,                    ///< Enable emergency.
      EmergencyOff = 3,                   ///< Disable emergency.
      PowerSelect = 4,                    ///< Select TX power.
      MonitorToggle = 5,                  ///< Toggle monitor (promiscuous mode on digital channel, open squelch on analog channel).
      OneTouch1 = 7,                      ///< Perform one-touch action 1.
      OneTouch2 = 8,                      ///< Perform one-touch action 2.
      OneTouch3 = 9,                      ///< Perform one-touch action 3.
      OneTouch4 = 10,                     ///< Perform one-touch action 4.
      OneTouch5 = 11,                     ///< Perform one-touch action 5.
      OneTouch6 = 12,                     ///< Perform one-touch action 6.
      RepeaterTalkaroundToggle = 13,      ///< Toggle repater mode / talkaround.
      ScanToggle = 14,                    ///< Start/stop scan.
      SquelchToggle = 21,                 ///< Enable/disable squelch.
      PrivacyToggle = 22,                 ///< Enable/disable privacy system.
      VoxToggle = 23,                     ///< Enable/disable VOX.
      ZoneIncrement = 24,                 ///< Switch to next zone.
      BatteryIndicator = 26,              ///< Show battery charge.
      LoneWorkerToggle = 31,              ///< Toggle lone-worker.
      RecordToggle = 34,                  ///< Enable/disable recording (dep. on firmware).
      RecordPlayback = 35,                ///< Start/stop playback.
      RecordDeleteAll = 36,               ///< Delete all recordings.
      Tone1750Hz = 38,                    ///< Send 1750Hz tone.
      SwitchUpDown = 47,                  ///< Switch Channel A/B.
      RightKey = 48,                      ///< Who knows?
      LeftKey = 49,                       ///< Who knows?
      ZoneDecrement = 55                  ///< Switch to previous zone.
    };

  protected:
    /** Hidden constructor. */
    ButtonSettingsElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit ButtonSettingsElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~ButtonSettingsElement();

    void clear();

    /** Returns the action for a short press on side button 1. */
    virtual ButtonAction sideButton1Short() const;
    /** Sets the action for a short press on side button 1. */
    virtual void sideButton1Short(ButtonAction action);
    /** Returns the action for a long press on side button 1. */
    virtual ButtonAction sideButton1Long() const;
    /** Sets the action for a short press on side button 1. */
    virtual void sideButton1Long(ButtonAction action);

    /** Returns the action for a short press on side button 2. */
    virtual ButtonAction sideButton2Short() const;
    /** Sets the action for a short press on side button 2. */
    virtual void sideButton2Short(ButtonAction action);
    /** Returns the action for a long press on side button 2. */
    virtual ButtonAction sideButton2Long() const;
    /** Sets the action for a short press on side button 2. */
    virtual void sideButton2Long(ButtonAction action);

    /** Returns the long-press duration in ms. */
    virtual uint longPressDuration() const;
    /** Sets the long-press duration in ms. */
    virtual void longPressDuration(uint ms);
  };

  /** Represents a single one-touch setting within the codeplug on the radio.
   *
   * Memory representation of a one-touch setting:
   * @verbinclude tytonetouchsetting.txt */
  class OneTouchSettingElement: public CodePlug::Element
  {
  public:
    /** Possible one-touch actions. */
    enum Action {
      CALL    = 0b0000,                 ///< Call someone, see @c contact.
      MESSAGE = 0b0001,                 ///< Send a message, see @c message.
      DTMF1   = 0b1000,                 ///< Analog call DTMF system 1.
      DTMF2   = 0b1001,                 ///< Analog call DTMF system 2.
      DTMF3   = 0b1010,                 ///< Analog call DTMF system 3.
      DTMF4   = 0b1011                  ///< Analog call DTMF system 4.
    };

    /** Possible one-touch action types. */
    enum Type {
      Disabled = 0b00,                  ///< Disabled one-touch.
      Digital  = 0b01,                  ///< Digital call/message.
      Analog   = 0b10                   ///< Analog call.
    };

  protected:
    /** Hidden constructor. */
    OneTouchSettingElement(uint8_t *ptr, size_t size);

  public:
    /** Constuctor. */
    explicit OneTouchSettingElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~OneTouchSettingElement();

    bool isValid() const;
    void clear();

    /** Returns the action to perform. */
    virtual Action action() const;
    /** Sets the action to perform. */
    virtual void action(Action action);

    /** Returns the type of the action. */
    virtual Type actionType() const;
    /** Sets the type of the action. */
    virtual void actionType(Type action);

    /** Returns the message index +1. */
    virtual uint8_t messageIndex() const;
    /** Sets the message index +1. */
    virtual void messageIndex(uint8_t idx);

    /** Returns the contact index +1. */
    virtual uint16_t contactIndex() const;
    /** Sets the contact index +1. */
    virtual void contactIndex(uint16_t idx);
  };

  /** Represents the emergency settings within the codeplug on the radio.
   *
   * Memory representation of the emergency settings:
   * @verbinclude tytemergencysettings.txt */
  class EmergencySettingsElement: public CodePlug::Element
  {
  protected:
    /** Hidden constructor. */
    EmergencySettingsElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit EmergencySettingsElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~EmergencySettingsElement();

    virtual void clear();

    /** Returns @c true if emergency remote monitor is enabled. */
    virtual bool emergencyRemoteMonitor() const;
    /** Enables/disables emergency remote monitor. */
    virtual void emergencyRemoteMonitor(bool enable);
    /** Returns @c true if remote monitor is enabled. */
    virtual bool remoteMonitor() const;
    /** Enables/disables remote monitor. */
    virtual void remoteMonitor(bool enable);
    /** Returns @c true if radio disable is enabled. */
    virtual bool radioDisable() const;
    /** Enables/disables radio disable. */
    virtual void radioDisable(bool enable);

    /** Returns the remote monitor duration in seconds. */
    virtual uint remoteMonitorDuration() const;
    /** Sets the remote monitor duration in seconds. */
    virtual void remoteMonitorDuration(uint sec);

    /** Returns the TX time-out in ms. */
    virtual uint txTimeOut() const;
    /** Sets the TX time-out in ms. */
    virtual void txTimeOut(uint ms);

    /** Returns the message limit. */
    virtual uint messageLimit() const;
    /** Sets the message limit. */
    virtual void messageLimit(uint limit);
  };

  /** Represents a single emergency system within the radio.
   *
   * Memory representation of emergency system:
   * @verbinclude tytemergencysystem.txt */
  class EmergencySystemElement: public CodePlug::Element
  {
  public:
    /** Possible alarm type for the system. */
    enum AlarmType {
      DISABLED = 0,                     ///< No alarm at all
      REGULAR = 1,                      ///< Regular alarm sound.
      SILENT = 2,                       ///< Silent alarm.
      SILENT_W_VOICE = 3                ///< silent alarm with voice.
    };
    /** Possible alarm modes for the system. */
    enum AlarmMode {
      ALARM = 0,                        ///< Just alarm.
      ALARM_W_CALL = 1,                 ///< Alarm + call.
      ALARM_W_VOICE = 2                 ///< Alarm + call + voice?
    };

  protected:
    /** Hidden constructor. */
    EmergencySystemElement(uint8_t *ptr, size_t size);

  public:
    /** Constructor. */
    explicit EmergencySystemElement(uint8_t *ptr);
    /** Destructor. */
    virtual ~EmergencySystemElement();

    bool isValid() const;
    void clear();

    /** Returns the name of the system. */
    virtual QString name() const;
    /** Sets the name of the system. */
    virtual void name(const QString &name);

    /** Returns the alarm type of the system. */
    virtual AlarmType alarmType() const;
    /** Sets the alarm type of the system. */
    virtual void alarmType(AlarmType type);
    /** Returns the alarm mode of the system. */
    virtual AlarmMode alarmMode() const;
    /** Sets the alarm mode of the system. */
    virtual void alarmMode(AlarmMode mode);

    /** Returns the number of impolite retries. */
    virtual uint impoliteRetries() const;
    /** Sets the number of impolite retries. */
    virtual void impoliteRetries(uint num);

    /** Returns the number of polite retries. */
    virtual uint politeRetries() const;
    /** Sets the number of polite retries. */
    virtual void politeRetries(uint num);

    /** Returns the hot MIC duration in seconds. */
    virtual uint hotMICDuration() const;
    /** Sets the hot MIC duration in seconds. */
    virtual void hotMICDuration(uint sec);

    /** Returns @c true if the revert channel is the selected one. */
    virtual bool revertChannelIsSelected() const;
    /** Returns the index of the revert channel. */
    virtual uint16_t revertChannelIndex() const;
    /** Sets the revert channel index. */
    virtual void revertChannelIndex(uint16_t idx);
    /** Sets revert channel to selected channel. */
    virtual void revertChannelSelected();
  };


protected:
  /** Empty constructor. */
  explicit TyTCodeplug(QObject *parent = nullptr);

public:
  /** Destructor. */
  virtual ~TyTCodeplug();

  /** Clears and resets the complete codeplug to some default values. */
  virtual void clear();

  /** Decodes the binary codeplug and stores its content in the given generic configuration. */
	bool decode(Config *config);
  /** Encodes the given generic configuration as a binary codeplug. */
  bool encode(Config *config, const Flags &flags = Flags());

public:
  /** Clears the time-stamp in the codeplug. */
  virtual void clearTimestamp();
  /** Sets the time-stamp. */
  virtual bool encodeTimestamp();

  /** Clears the general settings in the codeplug. */
  virtual void clearGeneralSettings();
  /** Updates the general settings from the given configuration. */
  virtual bool encodeGeneralSettings(Config *config, const Flags &flags);
  /** Updates the given configuration from the general settings. */
  virtual bool decodeGeneralSettings(Config *config);

  /** Clears all contacts in the codeplug. */
  virtual void clearContacts();
  /** Encodes all digital contacts in the configuration into the codeplug. */
  virtual bool encodeContacts(Config *config, const Flags &flags);
  /** Adds a digital contact to the configuration for each one in the codeplug. */
  virtual bool createContacts(CodeplugContext &ctx);

  /** Clears all RX group lists in the codeplug. */
  virtual void clearGroupLists();
  /** Encodes all group lists in the configuration into the codeplug. */
  virtual bool encodeGroupLists(Config *config, const Flags &flags);
  /** Adds a RX group list to the configuration for each one in the codeplug. */
  virtual bool createGroupLists(CodeplugContext &ctx);
  /** Links all added RX group lists within the configuration. */
  virtual bool linkGroupLists(CodeplugContext &ctx);

  /** Clears all channels in the codeplug. */
  virtual void clearChannels();
  /** Encodes all channels in the configuration into the codeplug. */
  virtual bool encodeChannels(Config *config, const Flags &flags);
  /** Adds a channel to the configuration for each one in the codeplug. */
  virtual bool createChannels(CodeplugContext &ctx);
  /** Links all added channels within the configuration. */
  virtual bool linkChannels(CodeplugContext &ctx);

  /** Clears all zones in the codeplug. */
  virtual void clearZones();
  /** Encodes all zones in the configuration into the codeplug. */
  virtual bool encodeZones(Config *config, const Flags &flags);
  /** Adds a zone to the configuration for each one in the codeplug. */
  virtual bool createZones(CodeplugContext &ctx);
  /** Links all added zones within the configuration. */
  virtual bool linkZones(CodeplugContext &ctx);

  /** Clears all scan lists in the codeplug. */
  virtual void clearScanLists();
  /** Encodes all scan lists in the configuration into the codeplug. */
  virtual bool encodeScanLists(Config *config, const Flags &flags);
  /** Adds a scan list to the configuration for each one in the codeplug. */
  virtual bool createScanLists(CodeplugContext &ctx);
  /** Links all added scan lists within the configuration. */
  virtual bool linkScanLists(CodeplugContext &ctx);

  /** Clears all positioning systems in the codeplug. */
  virtual void clearPositioningSystems();
  /** Encodes all DMR positioning systems in the configuration into the codeplug. */
  virtual bool encodePositioningSystems(Config *config, const Flags &flags);
  /** Adds a GPS positioning system to the configuration for each one in the codeplug. */
  virtual bool createPositioningSystems(CodeplugContext &ctx);
  /** Links all added positioning systems within the configuration. */
  virtual bool linkPositioningSystems(CodeplugContext &ctx);

  /** Clears the boot settings in the codeplug. */
  virtual void clearBootSettings();
  /** Clears the menu settings in the codeplug. */
  virtual void clearMenuSettings();
  /** Clears the button settings in the codeplug. */
  virtual void clearButtonSettings();
  /** Clears all text messages in the codeplug. */
  virtual void clearTextMessages();
  /** Clears all encryption keys in the codeplug. */
  virtual void clearPrivacyKeys();
  /** Clears all emergency systems in the codeplug. */
  virtual void clearEmergencySystems();
  /** Clears the VFO settings in the codeplug. */
  virtual void clearVFOSettings();
};

#endif // TYT_CODEPLUG_HH

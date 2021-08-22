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
   * Memmory layout of encoded channel:
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
   * Memmory layout of encoded contact:
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
   * Memmory layout of encoded zone:
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
   * Memmory layout of encoded zone extension:
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
   * Memmory layout of encoded RX group list:
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
   * Memmory layout of encoded scan list:
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
   * Memmory layout of encoded settings:
   * @verbinclude tytsettings.txt */
  class GeneralSettingsElement: CodePlug::Element
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
  class BootSettingsElement: CodePlug::Element
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
   * Memmory layout of encoded timestamp:
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
   * Memmory layout of encoded GPS system:
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

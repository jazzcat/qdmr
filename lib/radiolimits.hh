/** @defgroup limits Radio Limits
 * This module collects all classes used to define the limits for each supported radio. That is,
 * a tree of objects that hold the limitations like string length, number of elements in a list etc.
 * for the various settings of a radio including their extensions.
 *
 * This system will replace the static @c Radio::Features struct.
 *
 * @ingroup conf */
#ifndef RADIOLIMITS_HH
#define RADIOLIMITS_HH

#include <QObject>
#include <QTextStream>
#include <QMetaType>
#include <QSet>

// Forward declaration
class Config;
class ConfigItem;
class ConfigObject;
class RadioLimits;


/** Collects the issues found during verification.
 * This class also tracks where the issues arise.
 *
 * @ingroup limits */
class RadioLimitContext
{
public:
  /** Represents a single issue found during verification. */
  class Message: public QTextStream
  {
  public:
    /** Defines the possible severity levels. */
    enum Severity {
      Hint,    ///< Just a hint, a working codplug will be assembled.
      Warning, ///< The codeplug gets changed but a working codeplug might be assembled.
      Critical ///< Assembly of the codeplug will fail or a non-functional codeplug might be created.
    };

  public:
    /** Constructs an empty message for the specified severity at the specified point of the
     * verification. */
    Message(Severity severity, const QStringList &stack);
    /** Copy constructor. */
    Message(const Message &other);

    /** Copy assignment. */
    Message &operator =(const Message &other);

    /** Formats the message. */
    QString format() const;

  protected:
    /** Holds the severity of the issue. */
    Severity _severity;
    /** Holds the item-stack (where the issue occured). */
    QStringList _stack;
    /** Holds the text message. */
    QString _message;
  };

public:
  /** Empty constructor. */
  explicit RadioLimitContext();

  /** Constructs a new message and puts it into the list of issues. */
  Message &newMessage(Message::Severity severity = Message::Hint);

protected:
  /** The current item stack. */
  QStringList _stack;
  /** The list of issues found. */
  QList<Message> _messages;
};


/** Abstract base class for all radio limits.
 *
 * @ingroup limits */
class RadioLimitElement: public QObject
{
  Q_OBJECT

protected:
  /** Hidden constructor. */
  explicit RadioLimitElement(QObject *parent=nullptr);

public:
  /** Verifies the given property of the specified item.
   * This method gets implemented by the specialized classes to implement the actual verification. */
  virtual bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const = 0;

public:
  /** Destructor. */
  virtual ~RadioLimitElement();
};


/** Represents an ignored element in the codeplug.
 *
 * Instances of this class might be used to inform the user about a configured feature not present
 * in the particular radio.
 *
 * @ingroup limits */
class RadioLimitIgnored: public RadioLimitElement
{
  Q_OBJECT

public:
  /** Possible notification levels. */
  enum Notification {
    Silent,    ///< The user will not be notified.
    Hint,      ///< The user receives a hint.
    Warning,   ///< The user receives a warning.
    Error      ///< A codeplug cannot be assembled.
  };

public:
  /** Constructor for a ignored setting verification element. */
  RadioLimitIgnored(Notification notify=Silent, QObject *parent=nullptr);

protected:
  /** Holds the level of the notification. */
  Notification _notification;
};


/** Base class to verify values.
 *
 * That is, the verification of strings, integers, floats, etc.
 * @ingroup limits */
class RadioLimitValue: public RadioLimitElement
{
  Q_OBJECT

protected:
  /** Hidden constructor. */
  explicit RadioLimitValue(QObject *parent=nullptr);
};


/** Checks a string valued property.
 *
 * Instances of this class can be used to verify string values. That is, checking the length of the
 * string and its encoding.
 *
 * @ingroup limits */
class RadioLimitString: public RadioLimitValue
{
  Q_OBJECT

public:
  /** Possible encoding of strings. */
  enum Encoding {
    ASCII,   ///< Just ASCII is allowed.
    Unicode  ///< Any Unicode character is allowed.
  };

public:
  /** Constructor.
   * @param minLen Specifies the minimum length of the string. If -1, check is disabled.
   * @param maxLen Specifies the maximum length of the string. If -1, check is disabled.
   * @param enc Specifies the allowed string encoding.
   * @param parent Specifies the QObject parent object. */
  RadioLimitString(int minLen, int maxLen, Encoding enc, QObject *parent=nullptr);

  bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const;

protected:
  /** Holds the minimum length of the string. If -1, the check is disabled. */
  int _minLen;
  /** Holds the maximum length of the string. If -1, the check is disabled. */
  int _maxLen;
  /** Holds the allowed character encoding. */
  Encoding _encoding;
};


/** Represents a limit for an unsigned integer value.
 *
 * @ingroup limits */
class RadioLimitUInt: public RadioLimitValue
{
  Q_OBJECT

public:
  /** Constructor.
   * @param minValue Specifies the minimum value. If -1, no check is performed.
   * @param maxValue Specifies the maximum value. If -1, no check is performed.
   * @param parent Specifies the QObject parent. */
  RadioLimitUInt(qint64 minValue=-1, qint64 maxValue=-1, QObject *parent=nullptr);

  bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const;

protected:
  /** Holds the minimum value. If -1, the check is disabled. */
  qint64 _minValue;
  /** Holds the maximum value. If -1, the check is disabled. */
  qint64 _maxValue;
};


/** Represents a limit for a set of enum values.
 * @ingroup limits */
class RadioLimitEnum: public RadioLimitValue
{
  Q_OBJECT

public:
  /** Constructor from initializer list of possible enum values. */
  RadioLimitEnum(const std::initializer_list<unsigned> &values, QObject *parent=nullptr);

  bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const;

protected:
  /** Holds the set of valid values. */
  QSet<unsigned> _values;
};


/** Represents a limit on frequencies in MHz.
 * @ingroup limits */
class RadioLimitFrequencies: public RadioLimitValue
{
  Q_OBJECT

public:
  /** Represents a single frequency range. */
  struct FrequencyRange {
    double min; ///< Lower frequency limit.
    double max; ///< Upper frequency limit.
    /** Constructs a frequency range from limits. */
    FrequencyRange(double lower, double upper);
    /** Constructs a frequency range from limits. */
    FrequencyRange(const std::pair<double, double> &limit);
    /** Returns @c true if @c f is inside this limit. */
    bool contains(double f) const;
  };

public:
  /** Empty constructor. */
  explicit RadioLimitFrequencies(QObject *parent=nullptr);
  /** Constructor from initializer list. */
  RadioLimitFrequencies(const std::initializer_list<std::pair<double, double>> &ranges, QObject *parent=nullptr);

  bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const;

protected:
  /** Holds the frequency ranges for the device. */
  QList<FrequencyRange> _frequencyRanges;
};


/** Represents the limits for a @c ConfigItem instance.
 *
 * That is, it holds the limits for every property of the @c ConfigItem instance. This class
 * provides a initializer list constructor for easy programmatic contruction of limits.
 *
 * @ingroup limits */
class RadioLimitItem: public RadioLimitElement
{
  Q_OBJECT

public:
  /** Empty constructor. */
  explicit RadioLimitItem(QObject *parent=nullptr);
  /** Constructor from initializer list.
   * The ownership of all passed elements are taken. */
  RadioLimitItem(const std::initializer_list<std::pair<QString, RadioLimitElement *> > &list, QObject *parent=nullptr);

  virtual bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const;
  /** Verifies the properties of the given item. */
  virtual bool verifyItem(const ConfigItem *item, RadioLimitContext &context) const;

protected:
  /** Holds the property <-> limits map. */
  QHash<QString, RadioLimitElement *> _elements;
};


/** Represents the limits for all properties of a @c ConfigObject instance.
 * @ingroup limits */
class RadioLimitObject: public RadioLimitItem
{
  Q_OBJECT

public:
  /** Empty constructor. */
  explicit RadioLimitObject(QObject *parent=nullptr);
  /** Constructor from initializer list.
   * The ownership of all passed elements are taken. */
  RadioLimitObject(const std::initializer_list<std::pair<QString,RadioLimitElement *> > &list, QObject *parent=nullptr);

  /** Verifies the properties of the given object. */
  virtual bool verifyObject(const ConfigObject *item, RadioLimitContext &context) const;
};


/** Dispatch by class.
 *
 * Sometimes, a property may hold objects of different type. In these cases, a dispatcher is needed
 * to specify which limits to apply based on the type of the object. This class implements this
 * dispatcher.
 *
 * @ingroup limits */
class RadioLimitObjects: public RadioLimitObject
{
  Q_OBJECT

public:
  /** Constructor from initializer list.
   *
   * A list of pairs of a @c QMetaObject and a @c RadioLimitObject must be given. The meta object
   * specifies the type of the @c ConfigObject and the associated @c RadioLimitObject the limits
   * for this type.
   *
   * A dispatch for Analog and DigitalChannel may look like
   * @code[*.cpp]
   * new RadioLimitObjects{
   *   { AnalogChannel::staticMetaObject, new RadioLimitObject{
   *       // Limits for analog channel objects
   *     } },
   *   {DigialChannel::staticMetaObject, new RadioLimitObject{
   *       // Limits for digital channel objects
   *     } }
   * };
   * @endcode
   */
  RadioLimitObjects(const std::initializer_list<std::pair<const QMetaObject&, RadioLimitObject *> > &list, QObject *parent=nullptr);

  bool verifyItem(const ConfigItem *item, RadioLimitContext &context) const;

protected:
  /** Maps class-names to object limits. */
  QHash<QString,  RadioLimitObject *> _types;
};


/** Specifies the limits for a list of @c ConfigObject instances.
 * @ingroup limits */
class RadioLimitList: public RadioLimitElement
{
  Q_OBJECT

public:
  /** Constructor.
   * @param minSize Specifies the minimum size of the list. If -1, no check is performed.
   * @param maxSize Specifies the maximum size of the list. If -1, no check is performed.
   * @param element Specifies the limits for all objects in the list. If the list contains instances
   *                of different ConfigObject types, use @c RadioLimitObjects dispatcher.
   * @param parent  Specifies the QObject parent. */
  RadioLimitList(int minSize, int maxSize, RadioLimitObject *element, QObject *parent=nullptr);

  bool verify(const ConfigItem *item, const QMetaProperty &prop, RadioLimitContext &context) const;

protected:
  /** Holds the minimum size of the list. */
  qint64 _minSize;
  /** Holds the maximum size of the list. */
  qint64 _maxSize;
  /** Holds the limits for all objects of the list. */
  RadioLimitObject *_element;
};


/** Represents the limits or the entire codeplug.
 *
 * Use @c Radio::limits to obtain an instance.
 * @ingroup limits */
class RadioLimits : public RadioLimitItem
{
  Q_OBJECT

public:
  /** Empty constructor. */
  explicit RadioLimits(QObject *parent = nullptr);
  /** Constructor from initializer list. */
  RadioLimits(const std::initializer_list<std::pair<QString,RadioLimitElement *> > &list, QObject *parent=nullptr);

  /** Verifies the given configuration. */
  bool verifyConfig(const Config *config, RadioLimitContext &context);
};

#endif // RADIOLIMITS_HH

#ifndef ENCRYPTIONEXTENSION_HH
#define ENCRYPTIONEXTENSION_HH

#include "configobject.hh"


/** Base class of all encryption keys.
 * @ingroup conf */
class EncryptionKey: public ConfigObject
{
  Q_OBJECT

  /** The key representation as a hex string. */
  Q_PROPERTY(QString key READ toHex WRITE fromHex)

protected:
  /** Hidden constructor. */
  explicit EncryptionKey(QObject *parent=nullptr);

  void clear();

  /** Creates a key from the given hex-string. */
  virtual void fromHex(const QString &hex);
  /** Converts a key to a hex string. */
  virtual QString toHex() const;

protected:
  /** Holds the key data.
   * The size depends on the key type. */
  QByteArray _key;
};


/** Represents a DMR (basic) encryption key.
 *
 * This is a 16bit key used for the DMR basic encryption method.
 *
 * @ingroup conf */
class DMREncryptionKey: public EncryptionKey
{
  Q_OBJECT

public:
  /** Empty constructor. */
  explicit DMREncryptionKey(QObject *parent=nullptr);

  ConfigItem *clone() const;
  virtual void fromHex(const QString &hex);
};


/** Represents an AES (enhanced) encryption key.
 *
 * This is a 128bit key used for the DMR enhanced encryption method.
 *
 * @ingroup conf */
class AESEncryptionKey: public EncryptionKey
{
  Q_OBJECT

public:
  /** Empty constructor. */
  explicit AESEncryptionKey(QObject *parent=nullptr);

  ConfigItem *clone() const;
  virtual void fromHex(const QString &hex);
};


/** The list of encryption keys.
 *
 * This list holds all encryption keys defined within the codeplug.
 *
 * @ingroup conf */
class EncryptionKeys: public ConfigObjectList
{
  Q_OBJECT

public:
  /** Empty constructor. */
  explicit EncryptionKeys(QObject *parent=nullptr);

  int add(ConfigObject *obj, int row);

  ConfigItem *allocateChild(const YAML::Node &node, ConfigItem::Context &ctx, const ErrorStack &err=ErrorStack());
};


/** An extension to the codeplug that implements DMR encryption keys.
 *
 * @warning Please note that this is a commercial feature and forbidden for HAM radio use.
 *
 * @ingroup conf */
class EncryptionExtension: public ConfigExtension
{
  Q_OBJECT

  /** The encryption keys. */
  Q_PROPERTY(EncryptionKeys* keys READ keys)

public:
  /** Default constructor without any keys. */
  explicit EncryptionExtension(QObject *parent=nullptr);

  /** Returns the list of encryption keys. */
  EncryptionKeys *keys() const;

protected:
  /** Owns the list of keys. */
  EncryptionKeys *_keys;
};

#endif // ENCRYPTIONEXTENSION_HH

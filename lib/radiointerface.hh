/** @defgroup rif Radio interfaces
 * Depending on the manufacturer or model, different radios have different computer-radio
 * interfaces. This module collects all classes that provide these interfaces. */
#ifndef RADIOINFERFACE_HH
#define RADIOINFERFACE_HH

#include <QObject>

/** Abstract radio interface.
 * A radion interface must provide means to communicate with the device. That is, open a connection
 * to the device, allow for reading and writing specific memory blocks.
 *
 * This class defines the common interface for all radio-interface classes, irrespective of the
 * actual communication protocol being used by the device.
 * @ingroup rif */
class RadioInterface
{
protected:
  /** Hidden constructor. */
  explicit RadioInterface();

public:
  /** Destructor. */
	virtual ~RadioInterface();

  /** Return @c true if a connection to the device has been established. */
	virtual bool isOpen() const = 0;
  /** Closes the connection to the device. */
	virtual void close() = 0;

  /** Returns a device identifier. */
	virtual QString identifier() = 0;

  virtual bool write_start(uint32_t bank, uint32_t addr)=0;

  /** Writes a chunk of @c data at the block-address @c bno (block number).
   * @param bno Specifies the block number to write to.
   * @param data Pointer to the actual data to be written.
   * @param nbytes Specifies the number of bytes to write.
   * @returns @c true on success. */
  virtual bool write(uint32_t bank, uint32_t addr, uint8_t *data, int nbytes) = 0;

  /** This function ends a series of @c write_block operations.
   * This function will be re-implemented by certain interfaces that need completion of write
   * operations (e.g., HID). By default the method does nothing. */
  virtual bool write_finish() = 0;

  virtual bool read_start(uint32_t bank, uint32_t addr) = 0;
  /** Reads a chunk of data from the block-address @c bno (block number).
   * @param bno Specifies the block number to read from.
   * @param data Pointer where to store the read data.
   * @param nbytes Specifies the number of bytes to read.
   * @returns @c true on success. */
  virtual bool read(uint32_t bank, uint32_t addr, uint8_t *data, int nbytes) = 0;

  /** This function ends a series of @c read_block operations.
   * This function will be re-implemented by certain interfaces that need completion of read
   * operations (e.g., HID). By default the method does nothing. */
  virtual bool read_finish() = 0;

  /** Some radios need to be rebooted after being read or programmed. This function
   * will be re-implemented by some interfaces (e.g., DFUDevice) to reboot the radio. By default
   * this function does nothing. */
	virtual bool reboot();

  /** Returns the last error message. */
  virtual const QString &errorMessage() const = 0;
};

#endif // RADIOINFERFACE_HH

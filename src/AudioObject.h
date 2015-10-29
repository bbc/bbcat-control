#ifndef __AUDIO_OBJECT__
#define __AUDIO_OBJECT__

#include <vector>

#include <bbcat-base/misc.h>

BBC_AUDIOTOOLBOX_START

/*--------------------------------------------------------------------------------*/
/** A minimal class representing an audio object
 */
/*--------------------------------------------------------------------------------*/
class AudioObject
{
public:
  AudioObject() {}
  virtual ~AudioObject() {}
  
  /*--------------------------------------------------------------------------------*/
  /** Comparison operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool operator == (const AudioObject& obj) const {return (GetID() == obj.GetID());}

  /*--------------------------------------------------------------------------------*/
  /** Get ID
   */
  /*--------------------------------------------------------------------------------*/
  virtual const std::string& GetID() const = 0;

  /*--------------------------------------------------------------------------------*/
  /** Get Name
   */
  /*--------------------------------------------------------------------------------*/
  virtual const std::string& GetName() const = 0;

  /*--------------------------------------------------------------------------------*/
  /** Get start channel
   */
  /*--------------------------------------------------------------------------------*/
  virtual uint_t GetStartChannel() const = 0;

  /*--------------------------------------------------------------------------------*/
  /** Get number of channels
   */
  /*--------------------------------------------------------------------------------*/
  virtual uint_t GetChannelCount() const = 0;

  /*--------------------------------------------------------------------------------*/
  /** Set/Get start time in ns
   */
  /*--------------------------------------------------------------------------------*/
  virtual void SetStartTime(uint64_t t) {UNUSED_PARAMETER(t);}
  virtual uint64_t GetStartTime() const = 0;

  /*--------------------------------------------------------------------------------*/
  /** Set/Get duration in ns
   */
  /*--------------------------------------------------------------------------------*/
  virtual void SetDuration(uint64_t t) {UNUSED_PARAMETER(t);}
  virtual uint64_t GetDuration() const = 0;

  /*--------------------------------------------------------------------------------*/
  /** Set/Get end time in ns
   */
  /*--------------------------------------------------------------------------------*/
  virtual void SetEndTime(uint64_t t) {UNUSED_PARAMETER(t);}
  virtual uint64_t GetEndTime() const = 0;

#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON object
   *
   * ADM audio objects contain extra information for the JSON representation
   */
  /*--------------------------------------------------------------------------------*/
  virtual void ToJSON(json_spirit::mObject& obj) const = 0;
#endif

  typedef std::vector<AudioObject *> LIST;
};

BBC_AUDIOTOOLBOX_END

#endif

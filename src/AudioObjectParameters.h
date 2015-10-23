#ifndef __AUDIO_OBJECT_PARAMETERS__
#define __AUDIO_OBJECT_PARAMETERS__

#include <bbcat-base/3DPosition.h>
#include <bbcat-base/NamedParameter.h>
#include <bbcat-base/ParameterSet.h>
#include <bbcat-base/RefCount.h>

BBC_AUDIOTOOLBOX_START

/*--------------------------------------------------------------------------------*/
/** A class containing the parameters for rendering audio objects
 *
 * Each input channel to the renderer requires one of these objects which may change
 * over time as parameters for that channel change
 */
/*--------------------------------------------------------------------------------*/
class ADMAudioObject;
class AudioObjectParameters
{
public:
  AudioObjectParameters();
  AudioObjectParameters(const AudioObjectParameters& obj);
#if ENABLE_JSON
  AudioObjectParameters(const json_spirit::mObject& obj);
#endif
  virtual ~AudioObjectParameters();

  /*--------------------------------------------------------------------------------*/
  /** Assignment operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& operator = (const AudioObjectParameters& obj);

#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Assignment operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& operator = (const json_spirit::mObject& obj) {return FromJSON(obj);}
  /*--------------------------------------------------------------------------------*/
  /** Assignment operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual AudioObjectParameters& FromJSON(const json_spirit::mObject& obj, bool reset = true);
#endif

  /*--------------------------------------------------------------------------------*/
  /** Comparison operator
   */
  /*--------------------------------------------------------------------------------*/
  virtual bool operator == (const AudioObjectParameters& obj) const;
  virtual bool operator != (const AudioObjectParameters& obj) const {return !operator == (obj);}

  /*--------------------------------------------------------------------------------*/
  /** Transform this object's position and return new copy
   */
  /*--------------------------------------------------------------------------------*/
  friend AudioObjectParameters operator * (const AudioObjectParameters& obj, const PositionTransform& transform);

  /*--------------------------------------------------------------------------------*/
  /** Transform this object's position
   */
  /*--------------------------------------------------------------------------------*/
  AudioObjectParameters& operator *= (const PositionTransform& transform);

  /*--------------------------------------------------------------------------------*/
  /** Get/Set channel
   */
  /*--------------------------------------------------------------------------------*/
  uint_t GetChannel()            const {return values.channel;}
  bool   GetChannel(uint_t& val) const {return GetParameter<>(Parameter_channel, values.channel, val);}
  void   SetChannel(uint_t  val)       {SetParameter<>(Parameter_channel, values.channel, val);}
  void   ResetChannel()                {ResetParameter<>(Parameter_channel, values.channel);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set physical position of this object
   *
   * @note position information is required for every channel
   */
  /*--------------------------------------------------------------------------------*/
  const Position& GetPosition()                    const {return position;}
  bool            GetPosition(Position& val)       const {return GetParameter<>(Parameter_position, position, val);}
  void            SetPosition(const Position& val)       {SetParameter<>(Parameter_position, position, val);}
  void            ResetPosition()                        {ResetParameter<>(Parameter_position, position);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set gain
   */
  /*--------------------------------------------------------------------------------*/
  double GetGain()            const {return values.gain;}
  bool   GetGain(double& val) const {return GetParameter<>(Parameter_gain, values.gain, val);}
  void   SetGain(double  val)       {SetParameter<>(Parameter_gain, values.gain, val);}
  void   ResetGain()                {ResetParameter<>(Parameter_gain, values.gain, 1.0);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set width
   */
  /*--------------------------------------------------------------------------------*/
  double GetWidth()            const {return values.width;}
  bool   GetWidth(double& val) const {return GetParameter<>(Parameter_width, values.width, val);}
  void   SetWidth(double  val)       {SetParameter<>(Parameter_width, values.width, val, &Limit0);}
  void   ResetWidth()                {ResetParameter<>(Parameter_width, values.width);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set depth
   */
  /*--------------------------------------------------------------------------------*/
  double GetDepth()            const {return values.depth;}
  bool   GetDepth(double& val) const {return GetParameter<>(Parameter_depth, values.depth, val);}
  void   SetDepth(double  val)       {SetParameter<>(Parameter_depth, values.depth, val, &Limit0);}
  void   ResetDepth()                {ResetParameter<>(Parameter_depth, values.depth);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set height
   */
  /*--------------------------------------------------------------------------------*/
  double GetHeight()            const {return values.height;}
  bool   GetHeight(double& val) const {return GetParameter<>(Parameter_height, values.height, val);}
  void   SetHeight(double  val)       {SetParameter<>(Parameter_height, values.height, val, &Limit0);}
  void   ResetHeight()                {ResetParameter<>(Parameter_height, values.height);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set diffuseness
   */
  /*--------------------------------------------------------------------------------*/
  double GetDiffuseness()            const {return values.diffuseness;}
  bool   GetDiffuseness(double& val) const {return GetParameter<>(Parameter_diffuseness, values.diffuseness, val);}
  void   SetDiffuseness(double  val)       {SetParameter<>(Parameter_diffuseness, values.diffuseness, val, Limit0to1);}
  void   ResetDiffuseness()                {ResetParameter<>(Parameter_diffuseness, values.diffuseness);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set delay
   */
  /*--------------------------------------------------------------------------------*/
  double GetDelay()            const {return values.delay;}
  bool   GetDelay(double& val) const {return GetParameter<>(Parameter_delay, values.delay, val);}
  void   SetDelay(double  val)       {SetParameter<>(Parameter_delay, values.delay, val, &Limit0);}
  void   ResetDelay()                {ResetParameter<>(Parameter_delay, values.delay);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set importance
   */
  /*--------------------------------------------------------------------------------*/
  int    GetImportance()         const {return values.importance;}
  bool   GetImportance(int& val) const {return GetParameter<>(Parameter_importance, values.importance, val);}
  void   SetImportance(int  val)       {SetParameter<>(Parameter_importance, values.importance, val, &LimitImportance);}
  void   ResetImportance()             {ResetParameter<>(Parameter_importance, values.importance);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set dialogue
   */
  /*--------------------------------------------------------------------------------*/
  int    GetDialogue()         const {return values.dialogue;}
  bool   GetDialogue(int& val) const {return GetParameter<>(Parameter_dialogue, values.dialogue, val);}
  void   SetDialogue(int  val)       {SetParameter<>(Parameter_dialogue, values.dialogue, val, &LimitDialogue);}
  void   ResetDialogue()             {ResetParameter<>(Parameter_dialogue, values.dialogue);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set channellock
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetChannelLock()          const {return (values.channellock != 0);}
  bool   GetChannelLock(bool& val) const {return GetBoolParameter<>(Parameter_channellock, values.channellock, val);}
  void   SetChannelLock(bool  val)       {SetParameter<>(Parameter_channellock, values.channellock, val);}
  void   ResetChannelLock()              {ResetParameter<>(Parameter_channellock, values.channellock);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set interact
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetInteract()          const {return (values.interact != 0);}
  bool   GetInteract(bool& val) const {return GetBoolParameter<>(Parameter_interact, values.interact, val);}
  void   SetInteract(bool  val)       {SetParameter<>(Parameter_interact, values.interact, val);}
  void   ResetInteract()              {ResetParameter<>(Parameter_interact, values.interact);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set interpolate
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetInterpolate()          const {return (values.interpolate != 0);}
  bool   GetInterpolate(bool& val) const {return GetBoolParameter<>(Parameter_interpolate, values.interpolate, val);}
  void   SetInterpolate(bool  val)       {SetParameter<>(Parameter_interpolate, values.interpolate, val);}
  void   ResetInterpolate()              {ResetParameter<>(Parameter_interpolate, values.interpolate);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set interpolation time (ns by default)
   */
  /*--------------------------------------------------------------------------------*/
  uint64_t GetInterpolationTime()              const {return values.interpolationtime;}
  double   GetInterpolationTimeS()             const {return ConvertNSToS(values.interpolationtime);}
  bool     GetInterpolationTime(uint64_t& val) const {return GetParameter<>(Parameter_interpolationtime, values.interpolationtime, val);}
  bool     GetInterpolationTimeS(double&  val) const {return GetParameter<>(Parameter_interpolationtime, values.interpolationtime, val, &ConvertNSToS);}
  void     SetInterpolationTime(uint64_t  val)       {SetParameter<>(Parameter_interpolationtime, values.interpolationtime, val);}
  void     SetInterpolationTimeS(double   val)       {SetParameter<>(Parameter_interpolationtime, values.interpolationtime, val, &ConvertSToNS);}
  void     ResetInterpolationTime()                  {ResetParameter<>(Parameter_interpolationtime, values.interpolationtime);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set onscreen
   */
  /*--------------------------------------------------------------------------------*/
  bool   GetOnScreen()          const {return (values.onscreen != 0);}
  bool   GetOnScreen(bool& val) const {return GetBoolParameter<>(Parameter_onscreen, values.onscreen, val);}
  void   SetOnScreen(bool val)        {SetParameter<>(Parameter_onscreen, values.onscreen, val);}
  void   ResetOnScreen()              {ResetParameter<>(Parameter_onscreen, values.onscreen);}

  /*--------------------------------------------------------------------------------*/
  /** Get/Set supplementary data
   */
  /*--------------------------------------------------------------------------------*/
  const ParameterSet& GetOtherValues()                        const {return othervalues;}
  bool                GetOtherValues(ParameterSet& val)       const {return GetParameter<>(Parameter_othervalues, othervalues, val);}
  void                SetOtherValues(const ParameterSet& val)
  {
    if (val.IsEmpty()) ResetParameter<>(Parameter_othervalues, othervalues);
    else               SetParameter<>(Parameter_othervalues, othervalues, val);
  }
  void                ResetOtherValues()                            {ResetParameter<>(Parameter_othervalues, othervalues);}

  /*--------------------------------------------------------------------------------*/
  /** Convert all parameters into text and store them in a ParameterSet object 
   *
   * @param set ParameterSet object to receive parameters
   */
  /*--------------------------------------------------------------------------------*/
  virtual void GetAll(ParameterSet& set, bool force = false) const;

  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a string
   */
  /*--------------------------------------------------------------------------------*/
  std::string ToString(bool pretty = false) const;

#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON object
   */
  /*--------------------------------------------------------------------------------*/
  virtual void ToJSON(json_spirit::mObject& obj, bool force = false) const;

  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON object
   */
  /*--------------------------------------------------------------------------------*/
  virtual json_spirit::mObject ToJSON(bool force = false) const {json_spirit::mObject obj; ToJSON(obj, force); return obj;}

  /*--------------------------------------------------------------------------------*/
  /** Operator overload
   */
  /*--------------------------------------------------------------------------------*/
  operator json_spirit::mObject() const {return ToJSON();}
 
  /*--------------------------------------------------------------------------------*/
  /** Convert parameters to a JSON string
   */
  /*--------------------------------------------------------------------------------*/
  std::string ToJSONString() const {return json_spirit::write(ToJSON(), json_spirit::pretty_print);}
#endif

  /*--------------------------------------------------------------------------------*/
  /** Parameter modifier class
   */
  /*--------------------------------------------------------------------------------*/
  class Modifier : public RefCountedObject
  {
  public:
    Modifier() : RefCountedObject() {}
    Modifier(const Modifier& obj);
    virtual ~Modifier() {}

    NAMEDPARAMETER(Quaternion,rotation);
    NAMEDPARAMETER(Position,position);
    NAMEDPARAMETERDEF(double,gain,1.0);
    NAMEDPARAMETERDEF(double,scale,1.0);

    typedef std::vector<RefCount<Modifier> > LIST;

    /*--------------------------------------------------------------------------------*/
    /** Assignment operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual Modifier& operator = (const Modifier& obj);

    /*--------------------------------------------------------------------------------*/
    /** Comparison operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual bool operator == (const Modifier& obj) const;

    /*--------------------------------------------------------------------------------*/
    /** Specific modifications
     */
    /*--------------------------------------------------------------------------------*/
    virtual void Modify(AudioObjectParameters& parameters, const ADMAudioObject *object = NULL) const;

#if ENABLE_JSON
    /*--------------------------------------------------------------------------------*/
    /** Assignment operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual Modifier& operator = (const json_spirit::mObject& obj) {return FromJSON(obj);}
    /*--------------------------------------------------------------------------------*/
    /** Assignment operator
     */
    /*--------------------------------------------------------------------------------*/
    virtual Modifier& FromJSON(const json_spirit::mObject& obj);

    /*--------------------------------------------------------------------------------*/
    /** Convert parameters to a JSON object
     */
    /*--------------------------------------------------------------------------------*/
    virtual void ToJSON(json_spirit::mObject& obj) const;

    /*--------------------------------------------------------------------------------*/
    /** Convert parameters to a JSON object
     */
    /*--------------------------------------------------------------------------------*/
    virtual json_spirit::mObject ToJSON() const {json_spirit::mObject obj; ToJSON(obj); return obj;}

    /*--------------------------------------------------------------------------------*/
    /** Operator overload
     */
    /*--------------------------------------------------------------------------------*/
    operator json_spirit::mObject() const {return ToJSON();}
 
    /*--------------------------------------------------------------------------------*/
    /** Convert parameters to a JSON string
     */
    /*--------------------------------------------------------------------------------*/
    std::string ToJSONString() const {return json_spirit::write(ToJSON(), json_spirit::pretty_print);}
#endif
  };

  /*--------------------------------------------------------------------------------*/
  /** Modify this object's parameters using a single modifier
   */
  /*--------------------------------------------------------------------------------*/
  AudioObjectParameters& Modify(const Modifier& modifier, const ADMAudioObject *object);

  /*--------------------------------------------------------------------------------*/
  /** Modify this object's parameters using a list of modifiers
   */
  /*--------------------------------------------------------------------------------*/
  AudioObjectParameters& Modify(const Modifier::LIST& list, const ADMAudioObject *object);

protected:
  void GetList(std::vector<INamedParameter *>& list);
  void InitialiseToDefaults();

  /*--------------------------------------------------------------------------------*/
  /** List of parameters for 'setbitmap'
   *
   * @note this list is COMPLETELY INDEPENDANT of the VALUES structure and the order is ONLY relevant to ParameterNames[]
   */
  /*--------------------------------------------------------------------------------*/
  typedef enum {
    Parameter_channel = 0,

    Parameter_position,

    Parameter_gain,

    Parameter_width,
    Parameter_depth,
    Parameter_height,

    Parameter_diffuseness,
    Parameter_delay,

    Parameter_importance,
    Parameter_dialogue,

    Parameter_channellock,
    Parameter_interact,
    Parameter_interpolate,
    Parameter_interpolationtime,
    Parameter_onscreen,

    Parameter_othervalues,
    
    Parameter_count,
  } Parameter_t;

  /*--------------------------------------------------------------------------------*/
  /** Limit functions for various parameters
   */
  /*--------------------------------------------------------------------------------*/
  static uint8_t  LimitImportance(const int& val)   {return LIMIT(val, 0, 10);}
  static uint8_t  LimitDialogue(const int& val)     {return LIMIT(val, 0, 2);}
  static uint_t   Limit0(const int& val)            {return MAX(val, 0);}              // at least 0
  static double   Limit0(const double& val)         {return MAX(val, 0.0);}            // at least 0
  static double   Limit0to1(const double& val)      {return LIMIT(val, 0.0, 1.0);}     // between 0 and 1
  static uint64_t ConvertSToNS(const double& val)   {return (uint64_t)MAX(val * 1.0e9, 0.0);}
  static double   ConvertNSToS(const uint64_t& val) {return (double)val * 1.0e-9;}
  
  /*--------------------------------------------------------------------------------*/
  /** Set parameter from value
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of value
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param val new value
   * @param limit optional function ptr to a limiting/converting function that will convert the supplied value to a usable parameter value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void SetParameter(Parameter_t p, T1& param, const T2& val, T1 (*limit)(const T2& val) = NULL) {param = limit ? (*limit)(val) : T1(val); MarkParameterSet(p);}

  /*--------------------------------------------------------------------------------*/
  /** Get parameter to value
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of value to be return
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter source
   * @param val destination value
   *
   * @return true if value has been set, false if it is at its default
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  bool GetParameter(Parameter_t p, const T1& param, T2& val, T2 (*convert)(const T1& val) = NULL) const {val = convert ? (*convert)(param) : T2(param); return IsParameterSet(p);}

  /*--------------------------------------------------------------------------------*/
  /** Get parameter to bool value
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of value to be return
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter source
   * @param val destination value
   *
   * @return true if value has been set, false if it is at its default
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool GetBoolParameter(Parameter_t p, const T1& param, bool& val) const {val = (param != 0); return IsParameterSet(p);}

  /*--------------------------------------------------------------------------------*/
  /** Reset parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of reset value
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param val reset value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void ResetParameter(Parameter_t p, T1& param, const T2& val) {param = val; MarkParameterReset(p);}

  /*--------------------------------------------------------------------------------*/
  /** Reset parameter to 'zero'
   *
   * Template parameters:
   * @param T1 type of parameter
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  void ResetParameter(Parameter_t p, T1& param) {param = T1(); MarkParameterReset(p);}

  /*--------------------------------------------------------------------------------*/
  /** Set parameter in ParameterSet from specified parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param parameters ParameterSet object to be populated
   * @param force true to force parameters to be set, even if they have not been set
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  bool GetParameterFromParameters(Parameter_t p, const T1& param, ParameterSet& parameters, bool force = false) const {
    if (force || IsParameterSet(p)) parameters.Set(ParameterNames[p], param);
    return (force || IsParameterSet(p));
  }

#if ENABLE_JSON
  /*--------------------------------------------------------------------------------*/
  /** Set parameter from JSON
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 intermediate type that JSON framework can manage
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param final destination of value
   * @param val intermediate value as read from JSON
   * @param obj JSON object
   * @param reset true to reset parameter to its default if it is not found in the JSON object
   * @param defval reset value if reset = true
   * @param limit optional function ptr to a limiting/converting function that will convert the JSON value to a usable parameter value
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void SetFromJSON(Parameter_t p, T1& param, T2& val, const json_spirit::mObject& obj, bool reset = false, const T1& defval = T1(), T1 (*limit)(const T2& val) = NULL)
  {
    json_spirit::mObject::const_iterator it;
    // try and find named item in object
    if ((it = obj.find(ParameterNames[p])) != obj.end())
    {
      // read value from JSON into intermediate value
      bbcat::FromJSON(it->second, val);
      // use intermediate value to set parameter
      SetParameter<>(p, param, val, limit);
    }
    // if not found, reset parameter to default
    else if (reset) ResetParameter<>(p, param, defval);
  }    

  /*--------------------------------------------------------------------------------*/
  /** Set JSON from parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter value
   * @param obj JSON object
   * @param force true to force parameters to be set, even if they have not been set
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1>
  void SetToJSON(Parameter_t p, const T1& param, json_spirit::mObject& obj, bool force = false) const
  {
    if (force || IsParameterSet(p)) obj[ParameterNames[p]] = bbcat::ToJSON(param);
  }    

  /*--------------------------------------------------------------------------------*/
  /** Set JSON from parameter
   *
   * Template parameters:
   * @param T1 type of parameter
   * @param T2 type of JSON storage
   *
   * Function parameters:
   * @param p Parameter_xxx parameter enumeration
   * @param param parameter value
   * @param obj JSON object
   * @param force true to force parameters to be set, even if they have not been set
   * @param convert function to convert T1 to T2
   */
  /*--------------------------------------------------------------------------------*/
  template<typename T1, typename T2>
  void SetToJSON(Parameter_t p, const T1& param, json_spirit::mObject& obj, bool force, T2 (*convert)(const T1& val)) const
  {
    if (force || IsParameterSet(p)) obj[ParameterNames[p]] = bbcat::ToJSON((*convert)(param));
  }    
#endif
  
  /*--------------------------------------------------------------------------------*/
  /** Mark parameter p as being set
   */
  /*--------------------------------------------------------------------------------*/
  void MarkParameterSet(Parameter_t p)   {setbitmap |=  (1U << p);}

  /*--------------------------------------------------------------------------------*/
  /** Mark parameter p as being *not* set (reset)
   */
  /*--------------------------------------------------------------------------------*/
  void MarkParameterReset(Parameter_t p) {setbitmap &= ~(1U << p);}

  /*--------------------------------------------------------------------------------*/
  /** Return whether parameter p has been set
   */
  /*--------------------------------------------------------------------------------*/
  bool IsParameterSet(Parameter_t p) const {return ((setbitmap & (1U << p)) != 0);}
  
  /*--------------------------------------------------------------------------------*/
  /** Structure of simple data type items
   *
   * @note must contain POD types ONLY!  Is zeroed by code hence no objects can be used.
   *
   * @note the ORDER of the entries is to aid structure packing as much as possible (big to small) because
   * the size of this structure is a massive influence on the size of block formats!
   */
  /*--------------------------------------------------------------------------------*/
  typedef struct {
    double   gain;
    double   width;
    double   height;
    double   depth;
    double   diffuseness;
    double   delay;
    uint64_t interpolationtime;
    uint_t   channel;
    uint8_t  importance;
    uint8_t  dialogue;
    uint8_t  channellock;
    uint8_t  interact;
    uint8_t  interpolate;
    uint8_t  onscreen;
  } VALUES;
      
protected:
  Position     position;
  VALUES       values;
  uint_t       setbitmap;                           // bitmap of values that (good up to 32 items)
  ParameterSet othervalues;                         // additional, arbitrary parameters

  static const char *ParameterNames[Parameter_count];
};

BBC_AUDIOTOOLBOX_END

#endif

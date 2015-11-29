
#include <stdlib.h>
#include <string.h>

#define DEBUG_LEVEL 1
#include "AudioObjectParameters.h"

BBC_AUDIOTOOLBOX_START

// order taken from Parameter_t enumeration
const PARAMETERDESC AudioObjectParameters::parameterdescs[Parameter_count] =
{
  {"channel",                "Channel number (0-based)"},

  {"duration",               "Block duration (ns)"},
  
  {"position",               "Channel position"},

  {"gain",                   "Channel gain (linear)"},

  {"width",                  "Channel width"},
  {"height",                 "Channel height"},
  {"depth",                  "Channel depth"},

  {"divergencebalance",      "Channel divergence balance (0-1)"},
  {"divergenceazimuth",      "Channel divergence azimuth (degrees)"},

  {"diffuseness",            "Channel diffuseness (0-1)"},
  {"delay",                  "Channel delay (seconds)"},

  {"importance",             "Channel importance (0-10)"},
  {"dialogue",               "Whether channel is dialogue (0, 1 or 2)"},

  {"channellock",            "Channel is locked to channel (speaker)"},
  {"channellockmaxdistance", "Channel is locked to channel (speaker) max distance"},
  {"interact",               "Channel can be interacted with"},
  {"interpolate",            "Interpolate channel metadata changes"},
  {"interpolationtime",      "Time for interpolation of channel metadata changes (ns)"},
  {"onscreen",               "Channel is on screen"},

  {"othervalues",            "Other channel values"},
};

AudioObjectParameters::AudioObjectParameters() : setbitmap(0),
                                                 excludedZones(NULL)
{
  InitialiseToDefaults();
}

AudioObjectParameters::AudioObjectParameters(const AudioObjectParameters& obj) : setbitmap(0),
                                                                                 excludedZones(NULL)
{
  InitialiseToDefaults();
  operator = (obj);
}

#if ENABLE_JSON
AudioObjectParameters::AudioObjectParameters(const json_spirit::mObject& obj) : setbitmap(0),
                                                                                excludedZones(NULL)
{
  InitialiseToDefaults();
  operator = (obj);
}
#endif

AudioObjectParameters::~AudioObjectParameters()
{
  // delete entire chain of excluded zones
  ResetExcludedZones();
}

/*--------------------------------------------------------------------------------*/
/** Initialise all parameters to defaults
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::InitialiseToDefaults()
{
  // reset all values to zero
  memset(&values, 0, sizeof(values));

  // explicitly reset those parameters whose reset values are not zero
  ResetPosition();
  ResetGain();
  ResetOtherValues();

  // delete entire chain of excluded zones
  ResetExcludedZones();
}

/*--------------------------------------------------------------------------------*/
/** Assignment operator
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters& AudioObjectParameters::operator = (const AudioObjectParameters& obj)
{
  // do not copy oneself
  if (&obj != this)
  {
    position    = obj.position;
    values      = obj.values;
    othervalues = obj.othervalues;
    setbitmap   = obj.setbitmap;

    // delete entire chain of excluded zones
    ResetExcludedZones();
    
    // copy chain of excluded zones from obj
    if (obj.excludedZones) excludedZones = new ExcludedZone(*obj.excludedZones);
  }
  
  return *this;
}

#if ENABLE_JSON
/*--------------------------------------------------------------------------------*/
/** Assignment operator
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters& AudioObjectParameters::FromJSON(const json_spirit::mObject& obj, bool reset)
{
  Position     pval;
  ParameterSet sval;
  double       dval;
  sint64_t     i64val;
  float        fval;
  int          ival;
  bool         bval;
  
  SetFromJSON<>(Parameter_channel, values.channel, ival, obj, reset, 0U, &Limit0u);
  SetFromJSON<>(Parameter_duration, values.duration, i64val, obj, reset, (uint64_t)0);
  SetFromJSON<>(Parameter_position, position, pval, obj, reset);
  SetFromJSON<>(Parameter_gain, values.gain, dval, obj, reset, 1.0);
  SetFromJSON<>(Parameter_width, values.width, fval, obj, reset, 0.f, &Limit0f);
  SetFromJSON<>(Parameter_depth, values.depth, fval, obj, reset, 0.f, &Limit0f);
  SetFromJSON<>(Parameter_height, values.height, fval, obj, reset, 0.f, &Limit0f);
  SetFromJSON<>(Parameter_divergencebalance, values.divergencebalance, fval, obj, reset, 0.f, &Limit0to1f);
  SetFromJSON<>(Parameter_divergenceazimuth, values.divergenceazimuth, fval, obj, reset, 0.f, &Limit0f);
  SetFromJSON<>(Parameter_diffuseness, values.diffuseness, fval, obj, reset, 0.f, &Limit0to1f);
  SetFromJSON<>(Parameter_delay, values.delay, fval, obj, reset, 0.f, &Limit0f);
  SetFromJSON<>(Parameter_importance, values.importance, ival, obj, reset, (uint8_t)0, &LimitImportance);
  SetFromJSON<>(Parameter_dialogue, values.dialogue, ival, obj, reset, (uint8_t)0, &LimitDialogue);
  SetFromJSON<>(Parameter_channellock, values.channellock, bval, obj, reset);
  SetFromJSON<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, fval, obj, reset, 0.f, &LimitMaxDistance);
  SetFromJSON<>(Parameter_interact, values.interact, bval, obj, reset);
  SetFromJSON<>(Parameter_interpolate, values.interpolate, bval, obj, reset);
  SetFromJSON<>(Parameter_interpolationtime, values.interpolationtime, i64val, obj, reset, (uint64_t)0);
  SetFromJSON<>(Parameter_onscreen, values.onscreen, bval, obj, reset);
  SetFromJSON<>(Parameter_othervalues, othervalues, sval, obj, reset);

  // delete existing list of excluded zones
  if (excludedZones)
  {
    delete excludedZones;
    excludedZones = NULL;
  }
  
  {
    json_spirit::mObject::const_iterator it, it2;
    
    // try and find zoneExclusion item in object
    if (((it = obj.find("excludedzones")) != obj.end()) && (it->second.type() == json_spirit::array_type))
    {
      json_spirit::mArray zones = it->second.get_array();
      uint_t i;

      for (i = 0; i < zones.size(); i++)
      {
        if (zones[i].type() == json_spirit::obj_type)
        {
          json_spirit::mObject obj = zones[i].get_obj();
          std::string name;
          float minx, miny, minz, maxx, maxy, maxz;

          // extract name and limits of excluded zone
          if (((it2 = obj.find("name")) != obj.end()) && bbcat::FromJSON(it2->second, name) &&
              ((it2 = obj.find("minx")) != obj.end()) && bbcat::FromJSON(it2->second, minx) &&
              ((it2 = obj.find("miny")) != obj.end()) && bbcat::FromJSON(it2->second, miny) &&
              ((it2 = obj.find("minz")) != obj.end()) && bbcat::FromJSON(it2->second, minz) &&
              ((it2 = obj.find("maxx")) != obj.end()) && bbcat::FromJSON(it2->second, maxx) &&
              ((it2 = obj.find("maxy")) != obj.end()) && bbcat::FromJSON(it2->second, maxy) &&
              ((it2 = obj.find("maxz")) != obj.end()) && bbcat::FromJSON(it2->second, maxz))
          {
            AddExcludedZone(name, minx, miny, minz, maxx, maxy, maxz);
          }
          else ERROR("Unable to extract excluded zones from JSON '%s'", json_spirit::write(it->second).c_str());
        }
      }
    }
  }
  
  return *this;
}
#endif

/*--------------------------------------------------------------------------------*/
/** Comparison operator
 */
/*--------------------------------------------------------------------------------*/
bool AudioObjectParameters::operator == (const AudioObjectParameters& obj) const
{
  return ((position == obj.position) &&
          (memcmp(&values, &obj.values, sizeof(obj.values)) == 0) &&
          Compare(excludedZones, obj.excludedZones) &&
          (othervalues == obj.othervalues));
}

/*--------------------------------------------------------------------------------*/
/** Merge another AudioObjectParameters into this one
 *
 * @note any values set in obj will over-write the ones in this object
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters& AudioObjectParameters::Merge(const AudioObjectParameters& obj)
{
  CopyIfSet<>(obj, Parameter_channel, values.channel, obj.values.channel);
  CopyIfSet<>(obj, Parameter_duration, values.duration, obj.values.duration);
  CopyIfSet<>(obj, Parameter_position, position, obj.position);
  CopyIfSet<>(obj, Parameter_gain, values.gain, obj.values.gain);
  CopyIfSet<>(obj, Parameter_width, values.width, obj.values.width);
  CopyIfSet<>(obj, Parameter_depth, values.depth, obj.values.depth);
  CopyIfSet<>(obj, Parameter_height, values.height, obj.values.height);
  CopyIfSet<>(obj, Parameter_divergencebalance, values.divergencebalance, obj.values.divergencebalance);
  CopyIfSet<>(obj, Parameter_divergenceazimuth, values.divergenceazimuth, obj.values.divergenceazimuth);
  CopyIfSet<>(obj, Parameter_diffuseness, values.diffuseness, obj.values.diffuseness);
  CopyIfSet<>(obj, Parameter_delay, values.delay, obj.values.delay);
  CopyIfSet<>(obj, Parameter_importance, values.importance, obj.values.importance);
  CopyIfSet<>(obj, Parameter_dialogue, values.dialogue, obj.values.dialogue);
  CopyIfSet<>(obj, Parameter_channellock, values.channellock, obj.values.channellock);
  CopyIfSet<>(obj, Parameter_channellockmaxdistance, values.channellockmaxdistance, obj.values.channellockmaxdistance);
  CopyIfSet<>(obj, Parameter_interact, values.interact, obj.values.interact);
  CopyIfSet<>(obj, Parameter_interpolate, values.interpolate, obj.values.interpolate);
  CopyIfSet<>(obj, Parameter_interpolationtime, values.interpolationtime, obj.values.interpolationtime);
  CopyIfSet<>(obj, Parameter_onscreen, values.onscreen, obj.values.onscreen);
  CopyIfSet<>(obj, Parameter_othervalues, othervalues, obj.othervalues);
  return *this;
}

/*--------------------------------------------------------------------------------*/
/** Add a single excluded zone to list
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::AddExcludedZone(const std::string& name, float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
  ExcludedZone *zone;

  if ((zone = new ExcludedZone) != NULL)
  {
    zone->SetName(name);
    zone->SetMinCorner(minx, miny, minz);
    zone->SetMaxCorner(maxx, maxy, maxz);

    // if chain already exists, append this one to the end
    if (excludedZones) excludedZones->Add(zone);
    // otherwise start the chain with this one
    else               excludedZones = zone;
  }                       
}

/*--------------------------------------------------------------------------------*/
/** Delete all excluded zones
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::ResetExcludedZones()
{
  if (excludedZones)
  {
    delete excludedZones;
    excludedZones = NULL;
  }
}

/*--------------------------------------------------------------------------------*/
/** Transform this object's position and return new copy
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters operator * (const AudioObjectParameters& obj, const PositionTransform& transform)
{
  AudioObjectParameters res = obj;
  res.SetPosition(obj.GetPosition() * transform);
  return res;
}

/*--------------------------------------------------------------------------------*/
/** Transform this object's position
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters& AudioObjectParameters::operator *= (const PositionTransform& transform)
{
  Position centre = GetPosition(), corner = centre;

  SetPosition(centre * transform);
 
  if (centre.polar)
  {
    corner.pos.az += GetWidth();
    corner.pos.el += GetHeight();
    corner.pos.d  += GetDepth();
  }
  else
  {
    corner.pos.x  += GetWidth();
    corner.pos.y  += GetHeight();
    corner.pos.z  += GetDepth();
  }

  corner = (corner * transform) - centre;

  if (centre.polar)
  {
    SetWidth(corner.pos.az);
    SetHeight(corner.pos.el);
    SetDepth(corner.pos.d);
  }
  else
  {
    SetWidth(corner.pos.x);
    SetHeight(corner.pos.y);
    SetDepth(corner.pos.z);
  }
  
  return *this;
}

/*--------------------------------------------------------------------------------*/
/** Convert all parameters into text and store them in a ParameterSet object 
 *
 * @param set ParameterSet object to receive parameters
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::GetAll(ParameterSet& set, bool force) const
{
  GetParameterFromParameters<>(Parameter_channel, values.channel, set, force);
  GetParameterFromParameters<>(Parameter_duration, values.duration, set, force);
  if (force || IsParameterSet(Parameter_position)) position.SetParameters(set, parameterdescs[Parameter_position].name);
  GetParameterFromParameters<>(Parameter_gain, values.gain, set, force);
  GetParameterFromParameters<>(Parameter_width, values.width, set, force);
  GetParameterFromParameters<>(Parameter_depth, values.depth, set, force);
  GetParameterFromParameters<>(Parameter_height, values.height, set, force);
  GetParameterFromParameters<>(Parameter_divergencebalance, values.divergencebalance, set, force);
  GetParameterFromParameters<>(Parameter_divergenceazimuth, values.divergenceazimuth, set, force);
  GetParameterFromParameters<>(Parameter_diffuseness, values.diffuseness, set, force);
  GetParameterFromParameters<>(Parameter_delay, values.delay, set, force);
  GetParameterFromParameters<>(Parameter_importance, values.importance, set, force);
  GetParameterFromParameters<>(Parameter_dialogue, values.dialogue, set, force);
  GetParameterFromParameters<>(Parameter_channellock, values.channellock, set, force);
  GetParameterFromParameters<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, set, force);
  GetParameterFromParameters<>(Parameter_interact, values.interact, set, force);
  GetParameterFromParameters<>(Parameter_interpolate, values.interpolate, set, force);
  GetParameterFromParameters<>(Parameter_interpolationtime, values.interpolationtime, set, force);
  GetParameterFromParameters<>(Parameter_onscreen, values.onscreen, set, force);
  GetParameterFromParameters<>(Parameter_othervalues, othervalues, set, force);

  const ExcludedZone *zone;
  if ((zone = excludedZones) != NULL)
  {
    ParameterSet zones;
    uint_t n = 0;

    while (zone)
    {
      ParameterSet zonerep;
      Position c1 = zone->GetMinCorner();
      Position c2 = zone->GetMaxCorner();

      zonerep.Set("name", zone->GetName());
      zonerep.Set("minx", c1.pos.x);
      zonerep.Set("miny", c1.pos.y);
      zonerep.Set("minz", c1.pos.z);
      zonerep.Set("maxx", c2.pos.x);
      zonerep.Set("maxy", c2.pos.y);
      zonerep.Set("maxz", c2.pos.z);
      
      zones.Set(StringFrom(n), zonerep);
      
      zone = zone->GetNext();
      n++;
    }

    set.Set("excludedzones", zones);
  }
}

/*--------------------------------------------------------------------------------*/
/** Get a list of parameters for this object
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::GetParameterDescriptions(std::vector<const PARAMETERDESC *>& list)
{
  AddParametersToList(parameterdescs, NUMBEROF(parameterdescs), list);
}

/*--------------------------------------------------------------------------------*/
/** Get a list of overrideable parameters for this object
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::GetOverrideableParameterDescriptions(std::vector<const PARAMETERDESC *>& list)
{
  // channel and othervalues CANNOT be overriden currently
  AddParametersToList(parameterdescs + 1, NUMBEROF(parameterdescs) - 2, list);
}

/*--------------------------------------------------------------------------------*/
/** Set parameter from string
 */
/*--------------------------------------------------------------------------------*/
bool AudioObjectParameters::SetValue(const std::string& name, const std::string& value)
{
  // only one of the sets will work and a compiler that does early abort optimisation
  // will improve the speed of this function but not break it
  return (SetFromValue<>(Parameter_gain, values.gain, name, value) ||
          SetFromValue<>(Parameter_duration, values.duration, name, value) ||
          SetFromValue<>(Parameter_width, values.width, name, value, &Limit0f) ||
          SetFromValue<>(Parameter_depth, values.depth, name, value, &Limit0f) ||
          SetFromValue<>(Parameter_height, values.height, name, value, &Limit0f) ||
          SetFromValue<>(Parameter_divergencebalance, values.divergencebalance, name, value, &Limit0to1f) ||
          SetFromValue<>(Parameter_divergenceazimuth, values.divergenceazimuth, name, value, &Limit0f) ||
          SetFromValue<>(Parameter_diffuseness, values.diffuseness, name, value, &Limit0to1f) ||
          SetFromValue<>(Parameter_delay, values.delay, name, value, &Limit0f) ||
          SetFromValueConv<uint8_t,int>(Parameter_importance, values.importance, name, value, &LimitImportance) ||
          SetFromValueConv<uint8_t,int>(Parameter_dialogue, values.dialogue, name, value, &LimitDialogue) ||
          SetFromValueConv<uint8_t,int>(Parameter_channellock, values.channellock, name, value, &LimitBool) ||
          SetFromValue<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, name, value, &LimitMaxDistance) ||
          SetFromValueConv<uint8_t,int>(Parameter_interact, values.interact, name, value, &LimitBool) ||
          SetFromValueConv<uint8_t,int>(Parameter_interpolate, values.interpolate, name, value, &LimitBool) ||
          SetFromValueConv<uint64_t,sint64_t>(Parameter_interpolationtime, values.interpolationtime, name, value, &Limit0u64) ||
          SetFromValueConv<uint8_t,int>(Parameter_onscreen, values.onscreen, name, value, &LimitBool));
}

/*--------------------------------------------------------------------------------*/
/** Get parameter as string
 */
/*--------------------------------------------------------------------------------*/
bool AudioObjectParameters::GetValue(const std::string& name, std::string& value) const
{
  // only one of the gets will work and a compiler that does early abort optimisation
  // will improve the speed of this function but not break it
  return (GetToValue<>(Parameter_channel, values.channel, name, value) ||
          GetToValue<>(Parameter_duration, values.duration, name, value) ||
          GetToValue<>(Parameter_gain, values.gain, name, value) ||
          GetToValue<>(Parameter_width, values.width, name, value) ||
          GetToValue<>(Parameter_depth, values.depth, name, value) ||
          GetToValue<>(Parameter_height, values.height, name, value) ||
          GetToValue<>(Parameter_divergencebalance, values.divergencebalance, name, value) ||
          GetToValue<>(Parameter_divergenceazimuth, values.divergenceazimuth, name, value) ||
          GetToValue<>(Parameter_diffuseness, values.diffuseness, name, value) ||
          GetToValue<>(Parameter_delay, values.delay, name, value) ||
          GetToValue<>(Parameter_importance, values.importance, name, value) ||
          GetToValue<>(Parameter_dialogue, values.dialogue, name, value) ||
          GetToValue<>(Parameter_channellock, values.channellock, name, value) ||
          GetToValue<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, name, value) ||
          GetToValue<>(Parameter_interact, values.interact, name, value) ||
          GetToValue<>(Parameter_interpolate, values.interpolate, name, value) ||
          GetToValue<>(Parameter_interpolationtime, values.interpolationtime, name, value) ||
          GetToValue<>(Parameter_onscreen, values.onscreen, name, value));
}

/*--------------------------------------------------------------------------------*/
/** Reset parameter by name
 */
/*--------------------------------------------------------------------------------*/
bool AudioObjectParameters::ResetValue(const std::string& name)
{
  // only one of the gets will work and a compiler that does early abort optimisation
  // will improve the speed of this function but not break it
  return (ResetValue<>(Parameter_channel, values.channel, name) ||
          ResetValue<>(Parameter_duration, values.duration, name) ||
          ResetValue<>(Parameter_position, position, name) ||
          ResetValue<>(Parameter_gain, values.gain, name, 1.0) ||
          ResetValue<>(Parameter_width, values.width, name) ||
          ResetValue<>(Parameter_depth, values.depth, name) ||
          ResetValue<>(Parameter_height, values.height, name) ||
          ResetValue<>(Parameter_divergencebalance, values.divergencebalance, name) ||
          ResetValue<>(Parameter_divergenceazimuth, values.divergenceazimuth, name) ||
          ResetValue<>(Parameter_diffuseness, values.diffuseness, name) ||
          ResetValue<>(Parameter_delay, values.delay, name) ||
          ResetValue<>(Parameter_importance, values.importance, name) ||
          ResetValue<>(Parameter_dialogue, values.dialogue, name) ||
          ResetValue<>(Parameter_channellock, values.channellock, name) ||
          ResetValue<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, name) ||
          ResetValue<>(Parameter_interact, values.interact, name) ||
          ResetValue<>(Parameter_interpolate, values.interpolate, name) ||
          ResetValue<>(Parameter_interpolationtime, values.interpolationtime, name) ||
          ResetValue<>(Parameter_onscreen, values.onscreen, name));
}

/*--------------------------------------------------------------------------------*/
/** Convert parameters to a string
 */
/*--------------------------------------------------------------------------------*/
std::string AudioObjectParameters::ToString(bool pretty) const
{
  ParameterSet params;

  GetAll(params);

  return params.ToString(pretty);
}

#if ENABLE_JSON
/*--------------------------------------------------------------------------------*/
/** Convert parameters to a JSON object
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::ToJSON(json_spirit::mObject& obj, bool force) const
{
  SetToJSON<>(Parameter_channel, (int)values.channel, obj, force);
  SetToJSON<>(Parameter_duration, (sint64_t)values.duration, obj, force);
  SetToJSON<>(Parameter_position, position, obj, force);
  SetToJSON<>(Parameter_gain, values.gain, obj, force);
  SetToJSON<>(Parameter_width, values.width, obj, force);
  SetToJSON<>(Parameter_depth, values.depth, obj, force);
  SetToJSON<>(Parameter_height, values.height, obj, force);
  SetToJSON<>(Parameter_diffuseness, values.diffuseness, obj, force);
  SetToJSON<>(Parameter_divergencebalance, values.divergencebalance, obj, force);
  SetToJSON<>(Parameter_divergenceazimuth, values.divergenceazimuth, obj, force);
  SetToJSON<>(Parameter_delay, values.delay, obj, force);
  SetToJSON<>(Parameter_importance, (int)values.importance, obj, force);
  SetToJSON<>(Parameter_dialogue, (int)values.dialogue, obj, force);
  SetToJSON<>(Parameter_channellock, values.channellock, obj, force);
  SetToJSON<>(Parameter_channellockmaxdistance, values.channellockmaxdistance, obj, force);
  SetToJSON<>(Parameter_interact, values.interact, obj, force);
  SetToJSON<>(Parameter_interpolate, values.interpolate, obj, force);
  SetToJSON<>(Parameter_interpolationtime, (sint64_t)values.interpolationtime, obj, force);
  SetToJSON<>(Parameter_onscreen, values.onscreen, obj, force);
  SetToJSON<>(Parameter_othervalues, othervalues, obj, force);
  
  // output all excluded zones
  const ExcludedZone *zone = GetFirstExcludedZone();
  if (zone)
  {
    json_spirit::mArray zones;
    while (zone)
    {
      json_spirit::mObject subobj;
      Position c1 = zone->GetMinCorner();
      Position c2 = zone->GetMaxCorner();

      subobj["name"] = zone->GetName();
      subobj["minx"] = c1.pos.x;
      subobj["miny"] = c1.pos.y;
      subobj["minz"] = c1.pos.z;
      subobj["maxx"] = c2.pos.x;
      subobj["maxy"] = c2.pos.y;
      subobj["maxz"] = c2.pos.z;

      zones.push_back(subobj);
    
      zone = zone->GetNext();
    }

    obj["excludedzones"] = zones;
  }
  
  DEBUG2(("JSON: %s", json_spirit::write(obj, json_spirit::pretty_print).c_str()));
}
#endif

/*----------------------------------------------------------------------------------------------------*/

AudioObjectParameters::Modifier::Modifier(const Modifier& obj) : RefCountedObject()
{
  operator = (obj);
}

/*--------------------------------------------------------------------------------*/
/** Assignment operator
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters::Modifier& AudioObjectParameters::Modifier::operator = (const Modifier& obj)
{
  if (&obj != this)
  {
    rotation = obj.rotation;
    position = obj.position;
    gain     = obj.gain;
    scale    = obj.scale;
  }
  return *this;
}

/*--------------------------------------------------------------------------------*/
/** Comparison operator
 */
/*--------------------------------------------------------------------------------*/
bool AudioObjectParameters::Modifier::operator == (const Modifier& obj) const
{
  return ((rotation == obj.rotation) &&
          (position == obj.position) &&
          (gain     == obj.gain)     &&
          (scale    == obj.scale));
}

#if ENABLE_JSON
/*--------------------------------------------------------------------------------*/
/** Assignment operator
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters::Modifier& AudioObjectParameters::Modifier::FromJSON(const json_spirit::mObject& obj)
{
  INamedParameter *parameters[] =
  {
    &rotation,
    &position,
    &gain,
    &scale,
  };
  bbcat::FromJSON(obj, parameters, NUMBEROF(parameters));
  return *this;
}

/*--------------------------------------------------------------------------------*/
/** Convert parameters to a JSON object
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::Modifier::ToJSON(json_spirit::mObject& obj) const
{
  const INamedParameter *parameters[] =
  {
    &rotation,
    &position,
    &gain,
    &scale,
  };
  bbcat::ToJSON(parameters, NUMBEROF(parameters), obj);
}
#endif

/*--------------------------------------------------------------------------------*/
/** Specific modifications
 */
/*--------------------------------------------------------------------------------*/
void AudioObjectParameters::Modifier::Modify(AudioObjectParameters& parameters, const AudioObject *object) const
{
  UNUSED_PARAMETER(parameters);
  UNUSED_PARAMETER(object);
}

/*----------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/** Modify this object's parameters using a single modifier
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters& AudioObjectParameters::Modify(const Modifier& modifier, const AudioObject *object)
{
  if (modifier.rotation.IsSet())
  {
    SetPosition(GetPosition() * modifier.rotation.Get());
    Position size(GetWidth(), GetHeight(), GetDepth());
    size *= modifier.rotation.Get();
    SetWidth(size.pos.x);
    SetHeight(size.pos.y);
    SetDepth(size.pos.z);
  }
  if (modifier.position.IsSet()) SetPosition(GetPosition() + modifier.position.Get());
  if (modifier.scale.IsSet())
  {
    SetPosition(GetPosition() * modifier.scale.Get());
    Position size(GetWidth(), GetHeight(), GetDepth());
    size *= modifier.scale.Get();
    SetWidth(size.pos.x);
    SetHeight(size.pos.y);
    SetDepth(size.pos.z);
  }
  if (modifier.gain.IsSet()) SetGain(GetGain() * modifier.gain.Get());

  // apply specific modifications (from derived classes)
  modifier.Modify(*this, object);

  return *this;
}

/*--------------------------------------------------------------------------------*/
/** Modify this object's parameters using a list of modifiers
 */
/*--------------------------------------------------------------------------------*/
AudioObjectParameters& AudioObjectParameters::Modify(const Modifier::LIST& list, const AudioObject *object)
{
  uint_t i;

  for (i = 0; i < list.size(); i++) Modify(*list[i].Obj(), object);

  return *this;
}

BBC_AUDIOTOOLBOX_END

/*
 ==============================================================================
 
 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.
 
 See LICENSE.txt for  more info.
 
 ==============================================================================
*/

#pragma once

#include "pluginterfaces/base/ibstream.h"
#
#include "IPlugAPIBase.h"
#include "IPlugVST3_Parameter.h"

using namespace Steinberg;
using namespace Vst;

/** Shared VST3 controller code */
class IPlugVST3ControllerBase
{
public:
    
  IPlugVST3ControllerBase() = default;
  IPlugVST3ControllerBase(const IPlugVST3ControllerBase&) = delete;
  IPlugVST3ControllerBase& operator=(const IPlugVST3ControllerBase&) = delete;
    
  void Initialize(IPlugAPIBase* pPlug, ParameterContainer& parameters, bool plugIsInstrument/*, bool midiIn*/)
  {
    if (pPlug->NPresets())
      parameters.addParameter(new IPlugVST3PresetParameter(pPlug->NPresets()));
    
    if (!plugIsInstrument)
      parameters.addParameter(mBypassParameter = new IPlugVST3BypassParameter());
    
    for (int i = 0; i < pPlug->NParams(); i++)
    {
      IParam *p = pPlug->GetParam(i);
      
      UnitID unitID = kRootUnitId;
      
      const char* paramGroupName = p->GetGroupForHost();
      
      if (CStringHasContents(paramGroupName))
      {
        for (int j = 0; j < pPlug->NParamGroups(); j++)
        {
          if (strcmp(paramGroupName, pPlug->GetParamGroupName(j)) == 0)
          {
            unitID = j + 1;
          }
        }
        
        if (unitID == kRootUnitId) // new unit, nothing found, so add it
        {
          unitID = pPlug->AddParamGroup(paramGroupName);
        }
      }
      
      Parameter* pVST3Parameter = new IPlugVST3Parameter(p, i, unitID);
      parameters.addParameter(pVST3Parameter);
    }

//    if (midiIn)
//    {
//      mParamGroups.Add("MIDI Controllers");
//      uinfo.id = unitID = mParamGroups.GetSize();
//      uinfo.parentUnitId = kRootUnitId;
//      uinfo.programListId = kNoProgramListId;
//      name.fromAscii("MIDI Controllers");
//      addUnit(new Unit(uinfo));
//
//      ParamID midiParamIdx = kMIDICCParamStartIdx;
//      UnitID midiControllersID = unitID;
//
//      char buf[32];
//
//      for (int chan = 0; chan < NUM_CC_CHANS_TO_ADD; chan++)
//      {
//        sprintf(buf, "Ch %i", chan+1);
//
//        mParamGroups.Add(buf);
//        uinfo.id = unitID = mParamGroups.GetSize();
//        uinfo.parentUnitId = midiControllersID;
//        uinfo.programListId = kNoProgramListId;
//        name.fromAscii(buf);
//        addUnit(new Unit(uinfo));
//
//        for (int i = 0; i < 128; i++)
//        {
//          name.fromAscii(ControlStr(i));
//          parameters.addParameter(name, STR16(""), 0, 0, 0, midiParamIdx++, unitID);
//        }
//
//        parameters.addParameter(STR16("Channel Aftertouch"), STR16(""), 0, 0, 0, midiParamIdx++, unitID);
//        parameters.addParameter(STR16("Pitch Bend"), STR16(""), 0, 0.5, 0, midiParamIdx++, unitID);
//      }
//    }
     /*
     if (NPresets())
     {
     ProgramListWithPitchNames* list = new ProgramListWithPitchNames(String("Factory Presets"), kPresetParam, kRootUnitId);
     
     for (int i = 0; i< NPresets(); i++)
     {
     list->addProgram(String(GetPresetName(i)));
     }
     
     //      char noteName[128];
     
     // TODO: GetMidiNote ? !
     
     //      for (int i = 0; i< 128; i++)
     //      {
     //        if (MidiNoteName(i, noteName))
     //        {
     //          name.fromAscii(noteName);
     //          list->setPitchName(0, i, name); // TODO: this will only set it for the first preset!
     //        }
     //      }
     
     addProgramList(list);
     }
     */
  }
  
  ParamValue PLUGIN_API getParamNormalized(IPlugAPIBase* pPlug, ParamID tag)
  {
    IParam* param = pPlug->GetParam(tag);
        
    if (param)
    {
      return param->GetNormalized();
    }
        
    return 0.0;
  }
    
  void PLUGIN_API setParamNormalized(IPlugAPIBase* pPlug, ParamID tag, ParamValue value)
  {
    if (tag >= kBypassParam)
    {
      switch (tag)
      {
        case kPresetParam:
        {
          pPlug->RestorePreset(pPlug->NPresets() * value);
          break;
        }
        default:
          break;
      }
    }
    else
    {
      IParam* pParam = pPlug->GetParam(tag);
      
      if (pParam)
      {
        pParam->SetNormalized(value);
        pPlug->OnParamChangeUI(tag, kHost);
        pPlug->SendParameterValueFromAPI(tag, value, true);
      }
    }
  }
  
public:
  IPlugVST3BypassParameter* mBypassParameter = nullptr;
};

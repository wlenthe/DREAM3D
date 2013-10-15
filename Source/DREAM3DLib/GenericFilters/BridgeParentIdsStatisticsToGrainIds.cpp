#include "BridgeParentIdsStatisticsToGrainIds.h"

#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/Math/DREAM3DMath.h"
#include "DREAM3DLib/Math/MatrixMath.h"
#include "DREAM3DLib/OrientationOps/OrientationOps.h"
#include "DREAM3DLib/Utilities/DREAM3DRandom.h"

#include "DREAM3DLib/StatisticsFilters/FindNeighbors.h"
#include "DREAM3DLib/GenericFilters/FindGrainPhases.h"

#include "DREAM3DLib/GenericFilters/RenameCellArray.h"
#include "DREAM3DLib/GenericFilters/LinkFieldMapToCellArray.h"
#include "DREAM3DLib/GenericFilters/CreateFieldArrayFromCellArray.h"

#define ERROR_TXT_OUT 1
#define ERROR_TXT_OUT1 1

#define NEW_SHARED_ARRAY(var, m_msgType, size)\
  boost::shared_array<m_msgType> var##Array(new m_msgType[size]);\
  m_msgType* var = var##Array.get();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
BridgeParentIdsStatisticsToGrainIds::BridgeParentIdsStatisticsToGrainIds() :
  AbstractFilter(),
  m_GrainIdsArrayName(DREAM3D::CellData::GrainIds),
  m_CellParentIdsArrayName(DREAM3D::CellData::ParentIds),
  m_ParentDensityArrayName(DREAM3D::CellData::ParentDensity),
  m_FieldParentIdsArrayName(DREAM3D::FieldData::ParentIds),
  m_AvgCAxisMisalignmentsArrayName(DREAM3D::FieldData::AvgCAxisMisalignments),
  m_NumGrainsPerParentArrayName(DREAM3D::FieldData::NumGrainsPerParent),
  m_LocalCAxisMisalignmentsArrayName(DREAM3D::FieldData::LocalCAxisMisalignments),
  m_UnbiasedLocalCAxisMisalignmentsArrayName(DREAM3D::FieldData::UnbiasedLocalCAxisMisalignments),
  m_NeighborListArrayName(DREAM3D::FieldData::NeighborList),
  m_CAxisMisalignmentListArrayName(DREAM3D::FieldData::CAxisMisalignmentList),
  m_CrystalStructuresArrayName(DREAM3D::EnsembleData::CrystalStructures),
  m_CalcAvgAvgWMTROnly(false),
  m_ParentDensity(NULL),
  m_FieldParentIds(NULL),
  m_AvgCAxisMisalignments(NULL),
  m_NumGrainsPerParent(NULL),
  m_LocalCAxisMisalignments(NULL),
  m_UnbiasedLocalCAxisMisalignments(NULL),
  m_NeighborList(NULL),
  m_CAxisMisalignmentList(NULL),
  m_CrystalStructures(NULL)
{
  m_OrientationOps = OrientationOps::getOrientationOpsVector();

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
BridgeParentIdsStatisticsToGrainIds::~BridgeParentIdsStatisticsToGrainIds()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void BridgeParentIdsStatisticsToGrainIds::setupFilterParameters()
{
  FilterParameterVector parameters;
  {
    FilterParameter::Pointer option = FilterParameter::New();
    option->setHumanLabel("Calculate Average Parent Average C-Axis Misalignment Using Grouped Microtexture Grains Only");
    option->setPropertyName("CalcAvgAvgWMTROnly");
    option->setWidgetType(FilterParameter::BooleanWidget);
    option->setValueType("bool");
    option->setUnits("");
    parameters.push_back(option);
  }

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void BridgeParentIdsStatisticsToGrainIds::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  /* Code to read the values goes between these statements */
  /* FILTER_WIDGETCODEGEN_AUTO_GENERATED_CODE BEGIN*/
  setCalcAvgAvgWMTROnly( reader->readValue("CalcAvgAvgWMTROnly", getCalcAvgAvgWMTROnly()) );
  /* FILTER_WIDGETCODEGEN_AUTO_GENERATED_CODE END*/
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int BridgeParentIdsStatisticsToGrainIds::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  writer->closeFilterGroup();
  writer->writeValue("CalcAvgAvgWMTROnly", getCalcAvgAvgWMTROnly() );
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void BridgeParentIdsStatisticsToGrainIds::dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles, bool afterLink)
{
  setErrorCondition(0);
  std::stringstream ss;
  VolumeDataContainer* m = getVolumeDataContainer();



  // Cell Data

  // Field Data
  if(afterLink == false)
  {
    GET_PREREQ_DATA(m, DREAM3D, FieldData, FieldParentIds, -302, int32_t, Int32ArrayType, fields, 1)

    if (m_CalcAvgAvgWMTROnly == true)
    {
      // Now we are going to get a "Pointer" to the NeighborList object out of the DataContainer
      m_NeighborList = NeighborList<int>::SafeObjectDownCast<IDataArray*, NeighborList<int>*>(m->getFieldData(DREAM3D::FieldData::NeighborList).get());
      if(m_NeighborList == NULL)
      {
        ss << "NeighborLists Array Not Initialized correctly" << std::endl;
        setErrorCondition(-304);
        addErrorMessage(getHumanLabel(), ss.str(), -1);
      }

      m_CAxisMisalignmentList = NeighborList<float>::SafeObjectDownCast<IDataArray*, NeighborList<float>*>(m->getFieldData(m_CAxisMisalignmentListArrayName).get());
      if(NULL == m_CAxisMisalignmentList)
      {
        ss.str("");
        ss << "MisalignmentLists Array Not Initialized correctly" << std::endl;
        setErrorCondition(-308);
        addErrorMessage(getHumanLabel(), ss.str(), -308);
      }
    }
    else if (m_CalcAvgAvgWMTROnly == false)
    {
      GET_PREREQ_DATA(m, DREAM3D, FieldData, AvgCAxisMisalignments, -303, float, FloatArrayType, fields, 1)
    }



  }
  if(afterLink == true)
  {
    CREATE_NON_PREREQ_DATA(m, DREAM3D, FieldData, NumGrainsPerParent, int32_t, Int32ArrayType, 0, fields, 1)
    if (m_CalcAvgAvgWMTROnly  == false)
    {
      CREATE_NON_PREREQ_DATA(m, DREAM3D, FieldData, LocalCAxisMisalignments, float, FloatArrayType, 0.0, fields, 1)
    }
    else if (m_CalcAvgAvgWMTROnly == true)
    {
      CREATE_NON_PREREQ_DATA(m, DREAM3D, FieldData, UnbiasedLocalCAxisMisalignments, float, FloatArrayType, 0.0, fields, 1)
    }
  }

 typedef DataArray<unsigned int> XTalStructArrayType;
 GET_PREREQ_DATA(m, DREAM3D, EnsembleData, CrystalStructures, -305, unsigned int, XTalStructArrayType, ensembles, 1)

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void BridgeParentIdsStatisticsToGrainIds::preflight()
{
  dataCheck(true, 1, 1, 1, false);

  VolumeDataContainer* m = getVolumeDataContainer();
  if(NULL == m)
  {
    setErrorCondition(-999);
    notifyErrorMessage("The DataContainer Object was NULL", -999);
    return;
  }

  RenameCellArray::Pointer rename_cell_array = RenameCellArray::New();
  rename_cell_array->setObservers(this->getObservers());
  rename_cell_array->setVolumeDataContainer(m);
  rename_cell_array->setMessagePrefix(getMessagePrefix());
  rename_cell_array->setSelectedCellArrayName(m_CellParentIdsArrayName);
  rename_cell_array->setNewCellArrayName(m_GrainIdsArrayName);
  rename_cell_array->preflight();
  int err1 = rename_cell_array->getErrorCondition();
  if (err1 < 0)
  {
    setErrorCondition(rename_cell_array->getErrorCondition());
    addErrorMessages(rename_cell_array->getPipelineMessages());
    return;
  }

  LinkFieldMapToCellArray::Pointer link_field_map_to_cell_array = LinkFieldMapToCellArray::New();
  link_field_map_to_cell_array->setObservers(this->getObservers());
  link_field_map_to_cell_array->setVolumeDataContainer(m);
  link_field_map_to_cell_array->setMessagePrefix(getMessagePrefix());
  link_field_map_to_cell_array->setSelectedCellDataArrayName(m_GrainIdsArrayName);
  link_field_map_to_cell_array->preflight();
  int err2 = link_field_map_to_cell_array->getErrorCondition();
  if (err2 < 0)
  {
    setErrorCondition(link_field_map_to_cell_array->getErrorCondition());
    addErrorMessages(link_field_map_to_cell_array->getPipelineMessages());
    return;
  }

  CreateFieldArrayFromCellArray::Pointer create_field_array_from_cell_array = CreateFieldArrayFromCellArray::New();
  create_field_array_from_cell_array->setObservers(this->getObservers());
  create_field_array_from_cell_array->setVolumeDataContainer(m);
  create_field_array_from_cell_array->setMessagePrefix(getMessagePrefix());
  create_field_array_from_cell_array->setSelectedCellArrayName(m_ParentDensityArrayName);
  create_field_array_from_cell_array->preflight();
  int err3 = create_field_array_from_cell_array->getErrorCondition();
  if (err3 < 0)
  {
    setErrorCondition(create_field_array_from_cell_array->getErrorCondition());
    addErrorMessages(create_field_array_from_cell_array->getPipelineMessages());
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void BridgeParentIdsStatisticsToGrainIds::execute()
{
  VolumeDataContainer* m = getVolumeDataContainer();
  if(NULL == m)
  {
    setErrorCondition(-999);
    notifyErrorMessage("The DataContainer Object was NULL", -999);
    return;
  }

  setErrorCondition(0);
  dataCheck(false, m->getTotalPoints(), m->getNumFieldTuples(), m->getNumEnsembleTuples(), false);
  if (getErrorCondition() < 0)
  {
    return;
  }

  int numgrains = m->getNumFieldTuples();
  std::vector<float> AvgCAxisMisalignments(numgrains,0.0f);
  std::vector<int32_t> fieldParentIds(numgrains,0);

  std::vector<std::vector<int> > afterNeighborList(numgrains);
  std::vector<std::vector<float> > afterCAxisMisalignmentList(numgrains);

  if (m_CalcAvgAvgWMTROnly == false)
  {
    for(int i=1;i<numgrains;i++)
    {
      AvgCAxisMisalignments[i] = m_AvgCAxisMisalignments[i];
      fieldParentIds[i] = m_FieldParentIds[i];
    }
  }
  else if (m_CalcAvgAvgWMTROnly == true)
  {
    NeighborList<int>& neighborlist = *m_NeighborList;
    NeighborList<float>& caxismisalignmentlist = *m_CAxisMisalignmentList;

    for(int i=1;i<numgrains;i++)
    {
      fieldParentIds[i] = m_FieldParentIds[i];
      afterNeighborList[i].resize(neighborlist[i].size());
      afterCAxisMisalignmentList[i].resize(neighborlist[i].size());
      for (size_t j = 0; j < neighborlist[i].size(); j++)
      {
        afterNeighborList[i][j] = neighborlist[i][j];
        afterCAxisMisalignmentList[i][j] = caxismisalignmentlist[i][j];
        float checkCA = afterCAxisMisalignmentList[i][j];
        float checkCA2 = caxismisalignmentlist[i][j];

      }
    }
  }

  RenameCellArray::Pointer rename_cell_array = RenameCellArray::New();
  rename_cell_array->setObservers(this->getObservers());
  rename_cell_array->setVolumeDataContainer(m);
  rename_cell_array->setMessagePrefix(getMessagePrefix());
  rename_cell_array->setSelectedCellArrayName(m_CellParentIdsArrayName);
  rename_cell_array->setNewCellArrayName(m_GrainIdsArrayName);
  rename_cell_array->execute();
  int err1 = rename_cell_array->getErrorCondition();
  if (err1 < 0)
  {
    setErrorCondition(rename_cell_array->getErrorCondition());
    addErrorMessages(rename_cell_array->getPipelineMessages());
    return;
  }

  LinkFieldMapToCellArray::Pointer link_field_map_to_cell_array = LinkFieldMapToCellArray::New();
  link_field_map_to_cell_array->setObservers(this->getObservers());
  link_field_map_to_cell_array->setVolumeDataContainer(m);
  link_field_map_to_cell_array->setMessagePrefix(getMessagePrefix());
  link_field_map_to_cell_array->setSelectedCellDataArrayName(m_GrainIdsArrayName);
  link_field_map_to_cell_array->execute();
  int err2 = link_field_map_to_cell_array->getErrorCondition();
  if (err2 < 0)
  {
    setErrorCondition(link_field_map_to_cell_array->getErrorCondition());
    addErrorMessages(link_field_map_to_cell_array->getPipelineMessages());
    return;
  }

  dataCheck(false, m->getNumCellTuples(), m->getNumFieldTuples(), m->getNumEnsembleTuples(), true);

  int numparents = m->getNumFieldTuples();
  std::vector<int32_t> NumMTRGrainsPerParent(numparents,0);

  if (m_CalcAvgAvgWMTROnly == false)
  {
    for(int i=1;i<numgrains;i++)
    {
      int parentid = fieldParentIds[i];
      m_NumGrainsPerParent[parentid]++;
      m_LocalCAxisMisalignments[parentid] += AvgCAxisMisalignments[i];
    }
  }
  else if (m_CalcAvgAvgWMTROnly == true)
  {
    for(int i=1;i<numgrains;i++)
    {
      int parentid = fieldParentIds[i];
      m_NumGrainsPerParent[parentid]++;
      for (size_t j = 0; j < afterNeighborList[i].size(); j++)
      {
        if (fieldParentIds[afterNeighborList[i][j]] == parentid)
        {
          NumMTRGrainsPerParent[parentid]++;
          m_UnbiasedLocalCAxisMisalignments[parentid] += afterCAxisMisalignmentList[i][j];
          float checkCA3 = afterCAxisMisalignmentList[i][j];

        }
      }
    }
  }

  for(int i=1;i<numparents;i++)
  {
    if (m_CalcAvgAvgWMTROnly == false) m_LocalCAxisMisalignments[i] /= m_NumGrainsPerParent[i];
    if (m_CalcAvgAvgWMTROnly == true)
    {
      if(NumMTRGrainsPerParent[i] > 0) m_UnbiasedLocalCAxisMisalignments[i] /= NumMTRGrainsPerParent[i];
      else m_UnbiasedLocalCAxisMisalignments[i] = 0.0f;
    }
  }

  CreateFieldArrayFromCellArray::Pointer create_field_array_from_cell_array = CreateFieldArrayFromCellArray::New();
  create_field_array_from_cell_array->setObservers(this->getObservers());
  create_field_array_from_cell_array->setVolumeDataContainer(m);
  create_field_array_from_cell_array->setMessagePrefix(getMessagePrefix());
  create_field_array_from_cell_array->setSelectedCellArrayName(m_ParentDensityArrayName);
  create_field_array_from_cell_array->execute();
  int err3 = create_field_array_from_cell_array->getErrorCondition();
  if (err3 < 0)
  {
    setErrorCondition(create_field_array_from_cell_array->getErrorCondition());
    addErrorMessages(create_field_array_from_cell_array->getPipelineMessages());
    return;
  }


  // If there is an error set this to something negative and also set a message
  notifyStatusMessage("BridgeParentIdsStatisticsToGrainIds Completed");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------


/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "SpinsWriter.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>

#include "MXA/Utilities/MXAFileInfo.h"
#include "MXA/Utilities/MXADir.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SpinsWriter::SpinsWriter() :
FileWriter(),
m_GrainIdsArrayName(DREAM3D::CellData::GrainIds),
m_GrainIds(NULL)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SpinsWriter::~SpinsWriter()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SpinsWriter::setupFilterParameters()
{
  std::vector<FilterParameter::Pointer> parameters;
  {
    FilterParameter::Pointer option = FilterParameter::New();
    option->setHumanLabel("Output File");
    option->setPropertyName("OutputFile");
    option->setWidgetType(FilterParameter::OutputFileWidget);
    option->setFileExtension("*.spin");
    option->setFileType("Sparks Spin File");
    option->setValueType("string");
    parameters.push_back(option);
  }
  setFilterParameters(parameters);
}
// -----------------------------------------------------------------------------
void SpinsWriter::writeFilterParameters(AbstractFilterParametersWriter* writer)
{
  writer->writeValue("OutputFile", getOutputFile() );
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SpinsWriter::dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles)
{
  setErrorCondition(0);
  std::stringstream ss;
  VoxelDataContainer* m = getVoxelDataContainer();

  GET_PREREQ_DATA(m, DREAM3D, CellData, GrainIds, ss, -300, int32_t, Int32ArrayType, voxels, 1)
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SpinsWriter::preflight()
{
  dataCheck(true, 1, 1, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int SpinsWriter::writeHeader()
{
    VoxelDataContainer* m = getVoxelDataContainer();
  if (NULL == m)
  {
    std::stringstream ss;
    ss << "DataContainer Pointer was NULL and Must be valid." << __FILE__ << "("<<__LINE__<<")";
    addErrorMessage(getHumanLabel(), ss.str(), -2);
    setErrorCondition(-1);
    return -1;
  }

  int64_t totalPoints = m->getTotalPoints();
  dataCheck(false, totalPoints, 1, 1);
  if (getErrorCondition() < 0)
  {
    return -40;
  }

  size_t udims[3] = {0,0,0};
  m->getDimensions(udims);
#if (CMP_SIZEOF_SIZE_T == 4)
  typedef int32_t DimType;
#else
  typedef int64_t DimType;
#endif
  DimType dims[3] = {
    static_cast<DimType>(udims[0]),
    static_cast<DimType>(udims[1]),
    static_cast<DimType>(udims[2]),
  };
  int64_t totalpoints = dims[0] * dims[1] * dims[2];


  std::ofstream outfile;
  outfile.open(getOutputFile().c_str(), std::ios_base::binary);
  if(!outfile)
  {
    std::cout << "Failed to open: " << getOutputFile() << std::endl;
    return -1;
  }

  outfile << "-" << std::endl;
  outfile << "3 dimension" << std::endl;
  outfile << totalpoints << " sites" << std::endl;
  outfile << "26 max neighbors" << std::endl;
  outfile << "0 " << udims[0] << " xlo xhi" << std::endl;
  outfile << "0 " << udims[1] << " ylo yhi" << std::endl;
  outfile << "0 " << udims[2] << " zlo zhi" << std::endl;
  outfile << std::endl;
  outfile << "Values" << std::endl;
  outfile << std::endl;
  outfile.close();
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int SpinsWriter::writeFile()
{
  VoxelDataContainer* m = getVoxelDataContainer();
  if (NULL == m)
  {
    std::stringstream ss;
    ss << "DataContainer Pointer was NULL and Must be valid." << __FILE__ << "("<<__LINE__<<")";
    addErrorMessage(getHumanLabel(), ss.str(), -2);
    setErrorCondition(-1);
    return -1;
  }

  int64_t totalPoints = m->getTotalPoints();
  dataCheck(false, totalPoints, 1, 1);
  if (getErrorCondition() < 0)
  {
    return -40;
  }

  size_t udims[3] = {0,0,0};
  m->getDimensions(udims);
#if (CMP_SIZEOF_SIZE_T == 4)
  typedef int32_t DimType;
#else
  typedef int64_t DimType;
#endif
  DimType dims[3] = {
    static_cast<DimType>(udims[0]),
    static_cast<DimType>(udims[1]),
    static_cast<DimType>(udims[2]),
  };
  int64_t totalpoints = dims[0] * dims[1] * dims[2];


  std::ofstream outfile;
  outfile.open(getOutputFile().c_str(), std::ios_base::binary | std::ios_base::app);
  if(!outfile)
  {
    std::cout << "Failed to open: " << getOutputFile() << std::endl;
    return -1;
  }

  // Buffer the output with 4096 Bytes which is typically the size of a "Block" on a
  // modern Hard Drive. This should speed up the writes considerably
  char buffer[4096];
  outfile.rdbuf()->pubsetbuf(buffer, 4096);
  for (int k = 0; k < totalpoints; k++)
  {
    outfile << k << " " << m_GrainIds[k] << "\n";
  }
  outfile.close();

  // If there is an error set this to something negative and also set a message
  notifyStatusMessage("Writing Ph File Complete");
  return 0;
}


/* ============================================================================
* Copyright (c) 2014 Michael A. Jackson (BlueQuartz Software)
* Copyright (c) 2014 Dr. Michael A. Groeber (US Air Force Research Laboratories)
* Copyright (c) 2015 William Lenthe
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
*                           FA8650-10-D-5210
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "SIMPLibPlugin.h"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPLibPlugin::SIMPLibPlugin() :
  m_PluginName(""),
  m_Version(""),
  m_Vendor(""),
  m_Location(""),
  m_Status("")
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPLibPlugin::~SIMPLibPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SIMPLibPlugin::getPluginName()
{
  return m_PluginName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SIMPLibPlugin::setPluginName(QString name)
{
  m_PluginName = name;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SIMPLibPlugin::getVersion()
{
  return m_Version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SIMPLibPlugin::setVersion(QString version)
{
  m_Version = version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SIMPLibPlugin::getVendor()
{
  return m_Vendor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SIMPLibPlugin::setVendor(QString vendor)
{
  m_Vendor = vendor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SIMPLibPlugin::getLocation()
{
  return m_Location;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SIMPLibPlugin::setLocation(QString filePath)
{
  m_Location = filePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SIMPLibPlugin::getStatus()
{
  return m_Status;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SIMPLibPlugin::setStatus(QString status)
{
  m_Status = status;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SIMPLibPlugin::getCitations()
{
  return "";
}


/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
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
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef _VTKRECTILINEARGRIDREADER_H_
#define _VTKRECTILINEARGRIDREADER_H_

#include <string.h> // needed for the ::memcpy function below

#include <QtCore/QString>
#include <QtCore/QtEndian>
#include <QtCore/QFile>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/IOFilters/FileReader.h"
#include "DREAM3DLib/DataArrays/DataArray.hpp"
#include "DREAM3DLib/Common/Constants.h"

/**
 * @brief The VtkFeatureIdReader class
 * @author
 * @version
 */
class DREAM3DLib_EXPORT VtkFeatureIdReader : public FileReader
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    DREAM3D_SHARED_POINTERS(VtkFeatureIdReader)
    DREAM3D_STATIC_NEW_MACRO(VtkFeatureIdReader)
    DREAM3D_TYPE_MACRO_SUPER(VtkFeatureIdReader, FileReader)


    virtual ~VtkFeatureIdReader();
    DREAM3D_FILTER_PARAMETER(QString, DataContainerName)
    Q_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)
    DREAM3D_FILTER_PARAMETER(QString, CellAttributeMatrixName)
    Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

    DREAM3D_FILTER_PARAMETER(QString, InputFile)
    Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

    DREAM3D_FILTER_PARAMETER(QString, FeatureIdScalarName)
    Q_PROPERTY(QString FeatureIdScalarName READ getFeatureIdScalarName WRITE setFeatureIdScalarName)

    DREAM3D_FILTER_PARAMETER(QString, FeatureIdsArrayName)
    Q_PROPERTY(QString FeatureIdsArrayName READ getFeatureIdsArrayName WRITE setFeatureIdsArrayName)

    virtual const QString getCompiledLibraryName();
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);
    virtual const QString getGroupName();
    virtual const QString getSubGroupName();
    virtual const QString getHumanLabel();

    virtual void setupFilterParameters();

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    virtual void preflight();


    // Normal Instance Variables that are not Filter Parameters
    DREAM3D_INSTANCE_STRING_PROPERTY(Comment)
    DREAM3D_INSTANCE_STRING_PROPERTY(DatasetType)
    DREAM3D_INSTANCE_PROPERTY(bool, FileIsBinary)


  signals:
    void updateFilterParameters(AbstractFilter* filter);
    void parametersChanged();
    void preflightAboutToExecute();
    void preflightExecuted();

  protected:
    VtkFeatureIdReader();

    void dataCheck();
    void updateCellInstancePointers();

    /**
     * @brief Reads the VTK header and sets the values that are described in the header
     * @return Error Condition. Negative is Error.
     */
    int readHeader();

    /**
     * @brief
     * @return Error Condition. Negative is Error.
     */
    virtual int readFile();

    /**
     *
     * @param input
     * @param value
     * @return Error Condition. Negative is Error.
     */
    int parseCoordinateLine(const char* input, size_t& value);

    /**
      * @brief Parses the byte size from a data set declaration line
      * @param text
      * @return
      */
    size_t parseByteSize(char text[256]);

    /**
      *
      */
    int ignoreData(QFile& in, int byteSize, char* type, size_t xDim, size_t yDim, size_t zDim);

    /**
      *
      */
    template<typename T>
    int skipVolume(QFile& inStream, int byteSize, size_t xDim, size_t yDim, size_t zDim, T& diff)
    {
      int err = 0;
      int64_t totalSize = (int64_t)xDim * (int64_t)yDim * (int64_t)zDim;
      if (getFileIsBinary() == true)
      {
        T* buffer = new T[totalSize];
        // Read all the xpoints in one shot into a buffer
        qint64 bytesRead = inStream.read(reinterpret_cast<char* > (buffer), (totalSize * sizeof(T)));
        if(bytesRead != (totalSize * sizeof(T)))
        {
          qDebug() << " ERROR READING BINARY FILE. Bytes read was not the same as func->xDim *. " << byteSize << "." << bytesRead
                   << " vs " << (totalSize * sizeof(T)) ;
          return -1;
        }
        if (totalSize > 1)
        {
          T t = buffer[totalSize - 1];
          T t1 = buffer[totalSize - 2];
          // Dont forget to byte swap since VTK Binary Files are explicitly Big Endian formatted
          t = qFromBigEndian(t);
          t1 = qFromBigEndian(t1);
          diff = t - t1;
        }
        else
        {
          diff = buffer[totalSize];
        }
        delete buffer;
      }
      else
      {
        T tmp;
        T t2;
        for (int64_t x = 0; x < totalSize; ++x)
        {
          if(x == 1)
          {
            t2 = tmp;
          }
          inStream >> tmp;
          if(x == 1)
          {
            diff = tmp - t2;
          }
        }
      }
      return err;
    }

    template<typename T>
    int skipVolume(QFile& inStream, int byteSize, size_t xDim, size_t yDim, size_t zDim)
    {
      int err = 0;
      if(getFileIsBinary() == true)
      {
        int64_t totalSize = (int64_t)xDim * (int64_t)yDim * (int64_t)zDim;
        T* buffer = new T[totalSize];
        // Read all the xpoints in one shot into a buffer
        qint64 bytesRead = inStream.read(reinterpret_cast<char* > (buffer), (totalSize * sizeof(T)));
        if(bytesRead != (totalSize * sizeof(T)))
        {
          qDebug() << " ERROR READING BINARY FILE. Bytes read was not the same as func->xDim *. " << byteSize << "." << bytesRead << " vs "
                   << (totalSize * sizeof(T)) ;
          return -1;
        }
        delete buffer;
      }
      else
      {
        T tmp;
        QTextStream in(&inStream);
        for (size_t z = 0; z < zDim; ++z)
        {
          for (size_t y = 0; y < yDim; ++y)
          {
            for (size_t x = 0; x < xDim; ++x)
            {
              in >> tmp;
            }
          }
        }
      }
      return err;
    }

  private:
    DEFINE_CREATED_DATAARRAY_VARIABLE(int32_t, FeatureIds)

    VtkFeatureIdReader(const VtkFeatureIdReader&); // Copy Constructor Not Implemented
    void operator=(const VtkFeatureIdReader&); // Operator '=' Not Implemented
};

#endif /* VTKRECTILINEARGRIDREADER_H_ */



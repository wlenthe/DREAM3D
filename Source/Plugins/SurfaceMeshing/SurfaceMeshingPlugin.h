/*
 * Your License or Copyright Information can go here
 */

#ifndef _SurfaceMeshing_H_
#define _SurfaceMeshing_H_

#include <QtCore/QObject>
#include <QtCore/QSettings>

#include "DREAM3DLib/Plugin/DREAM3DPluginInterface.h"


/**
 * @class SurfaceMeshingPlugin SurfaceMeshingPlugin.h SurfaceMeshing/SurfaceMeshingPlugin.h
 * @brief
 * @author Michael A. Jackson for BlueQuartz Software
 * @date May 10, 2012
 * @version 1.0
 */
class SurfaceMeshingPlugin : public QObject, public DREAM3DPluginInterface
{
    Q_OBJECT

    Q_INTERFACES(DREAM3DPluginInterface)

  public:
    SurfaceMeshingPlugin();
    virtual ~SurfaceMeshingPlugin();
    /**
     * @brief Returns the name of the plugin
     */
    virtual QString getPluginName();

    /**
     * @brief Register all the filters with the FilterWidgetFactory
     */
  //  virtual void registerFilterWidgets();

    /**
    * @brief This registers the filters that this plugin implements with the Filter Manager that is passed in
    * @param fm The FilterManager to register the filters into.
    */
    virtual void registerFilters(FilterManager* fm);


    /**
     * @brief Writes the settings in the input gui to the Application's preference file
     * @param prefs A valid QSettings pointer.
     */
    virtual void writeSettings(QSettings& prefs);

    /**
     * @brief Reads the settings from the Application's preference file and sets
     * the input GUI widgets accordingly.
     * @param prefs
     */
    virtual void readSettings(QSettings& prefs);

  private:
    SurfaceMeshingPlugin(const SurfaceMeshingPlugin&); // Copy Constructor Not Implemented
    void operator=(const SurfaceMeshingPlugin&); // Operator '=' Not Implemented
};

#endif /* _SurfaceMeshing_H_ */

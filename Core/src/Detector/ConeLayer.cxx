///////////////////////////////////////////////////////////////////
// ConeLayer.cxx, ACTS project
///////////////////////////////////////////////////////////////////

// Geometry module
#include "ACTS/Layers/ConeLayer.h"

#include "ACTS/Utilities/Definitions.h"
#include "ACTS/Surfaces/ConeBounds.h"
#include "ACTS/EventData/TrackParameters.h"
// Core module
                
Acts::ConeLayer::ConeLayer(std::shared_ptr<Acts::Transform3D> transform,
                          std::shared_ptr<const Acts::ConeBounds> cbounds,
                          Acts::SurfaceArray* surfaceArray,
                          double thickness,
                          Acts::OverlapDescriptor* olap,
                          int laytyp) :
  ConeSurface(transform, cbounds),
  Layer(surfaceArray, thickness, olap, nullptr, laytyp)
{}

Acts::ConeLayer::ConeLayer(const Acts::ConeLayer& clay, const Acts::Transform3D& transf):
  ConeSurface(clay,transf),
  Layer(clay)
{}
    
const Acts::ConeSurface& Acts::ConeLayer::surfaceRepresentation() const
{
  return (*this);
}

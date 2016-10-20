// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// DigitizationModule.cpp, ACTS project
///////////////////////////////////////////////////////////////////

#include "ACTS/Digitization/DigitizationModule.hpp"

Acts::DigitizationModule::DigitizationModule(
    std::shared_ptr<const Segmentation> moduleSegmentation,
    double                              halfThickness,
    int                                 readoutDirection,
    double                              lorentzAngle)
  :

  m_halfThickness(halfThickness)
  , m_readoutDirection(readoutDirection)
  , m_lorentzAngle(lorentzAngle)
  , m_tanLorentzAngle(tan(lorentzAngle))
  , m_segmentation(moduleSegmentation)
  , m_boundarySurfaces()
  , m_segmentationSurfacesX()
  , m_segmentationSurfacesY()
{
  m_segmentation->createSegmenationSurfaces(m_boundarySurfaces,
                                            m_segmentationSurfacesX,
                                            m_segmentationSurfacesY,
                                            halfThickness,
                                            readoutDirection,
                                            lorentzAngle);
}

const Acts::SurfacePtrVector
Acts::DigitizationModule::segmentationSurfaces(
    const Acts::DigitizationCell& entryCids,
    const Acts::DigitizationCell& exitCids) const
{
  SurfacePtrVector sSurfaces;

  auto startbinX = entryCids.first;
  auto endbinX   = exitCids.first;
  // swap if needed
  if (startbinX > endbinX) std::swap(startbinX, endbinX);
  // now cash in the rest
  for (; startbinX <= endbinX; ++startbinX)
    sSurfaces.push_back(m_segmentationSurfacesX[startbinX]);

  // start bin, end bin
  auto startbinY = entryCids.second;
  auto endbinY   = exitCids.second;
  // swap if needed
  if (startbinY > endbinY) std::swap(startbinY, endbinY);
  // now cash in the rest
  for (; startbinY <= endbinY; ++startbinY)
    sSurfaces.push_back(m_segmentationSurfacesY[startbinY]);

  // return what you have
  return sSurfaces;
}

const Acts::SurfacePtrVector
Acts::DigitizationModule::stepSurfaces(const Vector3D& start,
                                       const Vector3D& end) const
{
  // prepare the return vector
  SurfacePtrVector stepSurfaces;

  const DigitizationCell startCell = m_segmentation->cell(start);
  const DigitizationCell endCell   = m_segmentation->cell(end);

  // go along x - first with the naive binning (i.e. w.o lorentz angle)
  size_t sCellX = startCell.first;
  size_t eCellX = endCell.first;
  if (sCellX > eCellX) std::swap(sCellX, eCellX);
  // now take the boundaries as well
  if (sCellX > 0) --sCellX;
  ++eCellX;  // @TODO check : safe because we can assume to have eCell+1
  // the surfaces along Y are easy, just the bin surfaces
  size_t sCellY = startCell.second;
  size_t eCellY = endCell.second;
  if (sCellY > eCellY) std::swap(sCellY, eCellY);
  // reserve - be safe
  stepSurfaces.reserve((eCellY - sCellY) + (eCellX - sCellX) + 2);
  // now fill the x surfaces
  for (; sCellX <= eCellX && sCellX < m_segmentationSurfacesX.size(); ++sCellX)
    stepSurfaces.push_back(m_segmentationSurfacesX[sCellX]);
  // end fill the y surfaces
  for (; sCellY <= eCellY && sCellY < m_segmentationSurfacesY.size(); ++sCellY)
    stepSurfaces.push_back(m_segmentationSurfacesY[sCellY]);
  // return the lot
  return stepSurfaces;
}
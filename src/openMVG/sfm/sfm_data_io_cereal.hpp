
// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#ifndef OPENMVG_SFM_DATA_IO_JSON_HPP
#define OPENMVG_SFM_DATA_IO_JSON_HPP

#include "openMVG/sfm/sfm_data_io.hpp"
#include <cereal/types/map.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

#include <iomanip>
#include <fstream>

namespace openMVG
{

template <
// JSONInputArchive/ ...
typename archiveType
>
bool Load_Cereal(
  SfM_Data & data,
  const std::string & filename,
  ESfM_Data flags_part)
{
  const bool bBinary = stlplus::extension_part(filename) == "bin";

  // List which part of the file must be considered
  const bool b_views = (flags_part & VIEWS) == VIEWS;
  const bool b_intrinsics = (flags_part & INTRINSICS) == INTRINSICS;
  const bool b_extrinsics = (flags_part & EXTRINSICS) == EXTRINSICS;
  const bool b_structure = (flags_part & STRUCTURE) == STRUCTURE;

  //Create the stream and check it is ok
  std::ifstream stream(filename.c_str());
  if (!stream.is_open())
    return false;

  // Data serialization
  {
    archiveType archive(stream);

    std::string version;
    archive(cereal::make_nvp("sfm_data_version", version));
    archive(cereal::make_nvp("root_path", data.s_root_path));

    if (b_views)
      archive(cereal::make_nvp("views", data.views));
    if (bBinary && !b_views) { // Binary file require read all the member
      Views views;
      archive(cereal::make_nvp("views", views));
    }

    if (b_intrinsics)
      archive(cereal::make_nvp("intrinsics", data.intrinsics));
    if (bBinary && !b_intrinsics) { // Binary file require read all the member
      Intrinsics intrinsics;
      archive(cereal::make_nvp("intrinsics", intrinsics));
    }

    if (b_extrinsics)
      archive(cereal::make_nvp("extrinsics", data.poses));
    if (bBinary && !b_extrinsics) { // Binary file require read all the member
      Poses poses;
      archive(cereal::make_nvp("extrinsics", poses));
    }

    // Structure -> See for export in another file
    if (b_structure)
      archive(cereal::make_nvp("structure", data.structure));
    if (bBinary && !b_structure) { // Binary file require read all the member
      Landmarks structure;
      archive(cereal::make_nvp("structure", structure));
    }
  }
  return true;
}

template <
// JSONOutputArchive/ ...
typename archiveType
>
bool Save_Cereal(
  const SfM_Data & data,
  const std::string & filename,
  ESfM_Data flags_part)
{
  // List which part of the file must be considered
  const bool b_views = (flags_part & VIEWS) == VIEWS;
  const bool b_intrinsics = (flags_part & INTRINSICS) == INTRINSICS;
  const bool b_extrinsics = (flags_part & EXTRINSICS) == EXTRINSICS;
  const bool b_structure = (flags_part & STRUCTURE) == STRUCTURE;

  //Create the stream and check it is ok
  std::ofstream stream(filename.c_str());
  if (!stream.is_open())
    return false;

  // Data serialization
  {
    archiveType archive(stream);
    const std::string version = "0.1";
    archive(cereal::make_nvp("sfm_data_version", version));
    archive(cereal::make_nvp("root_path", data.s_root_path));

    if (b_views)
      archive(cereal::make_nvp("views", data.views));

    if (b_intrinsics)
      archive(cereal::make_nvp("intrinsics", data.intrinsics));

    if (b_extrinsics)
      archive(cereal::make_nvp("extrinsics", data.poses));

    // Structure -> See for export in another file
    if (b_structure)
      archive(cereal::make_nvp("structure", data.structure));
  }
  return true;
}

/*
bool Load_JSON(
  SfM_Data & data,
  const std::string & filename,
  ESfM_Data flags_part)
{
  const std::string ext = stlplus::extension_part(filename);
  if (ext != "json")
    return false;

  const bool bload_views = (flags_part & VIEWS) == VIEWS;
  const bool bload_intrinsics = (flags_part & INTRINSICS) == INTRINSICS;
  const bool bload_extrinsics = (flags_part & EXTRINSICS) == EXTRINSICS;
  const bool bload_structure = (flags_part & STRUCTURE) == STRUCTURE;

  std::ifstream is(filename.c_str());
  if (!is.is_open())
    return false;

  // Loading ...
  {
    cereal::JSONInputArchive archive(is);

    std::string version;
    archive(cereal::make_nvp("sfm_data_version", version));
    archive(cereal::make_nvp("root_path", data.s_root_path));

    if (bload_views)
      archive(cereal::make_nvp("views", data.views));

    if (bload_intrinsics)
      archive(cereal::make_nvp("intrinsics", data.intrinsics));

    if (bload_extrinsics)
      archive(cereal::make_nvp("extrinsics", data.poses));

    // Structure -> See for export in another file
    if (bload_structure)
      archive(cereal::make_nvp("structure", data.structure));
  }
  return true;
}

bool Save_JSON(
  const SfM_Data & data,
  const std::string & filename,
  ESfM_Data flags_part)
{
  const std::string ext = stlplus::extension_part(filename);
  if (ext != "json")
    return false;

  const bool bsave_views = (flags_part & VIEWS) == VIEWS;
  const bool bsave_intrinsics = (flags_part & INTRINSICS) == INTRINSICS;
  const bool bsave_extrinsics = (flags_part & EXTRINSICS) == EXTRINSICS;
  const bool bsave_structure = (flags_part & STRUCTURE) == STRUCTURE;

  std::ofstream os(filename.c_str());
  if (!os.is_open())
    return false;

  cereal::JSONOutputArchive archive(os);

  std::string version;
  archive(cereal::make_nvp("sfm_data_version", version));
  archive(cereal::make_nvp("root_path", data.s_root_path));

  if (bsave_views)
    archive(cereal::make_nvp("views", data.views));
  if (bsave_intrinsics)
    archive(cereal::make_nvp("intrinsics", data.intrinsics));
  if (bsave_extrinsics)
    archive(cereal::make_nvp("extrinsics", data.poses));
  if (bsave_structure)
    archive(cereal::make_nvp("structure", data.structure));

  return true;
}
*/
} // namespace openMVG

#endif // OPENMVG_SFM_DATA_IO_JSON_HPP
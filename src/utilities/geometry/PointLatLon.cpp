/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PointLatLon.hpp"
#include "Point3d.hpp"

#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/UTMUPS.hpp>

namespace openstudio {

/// default constructor creates point at 0, 0, 0
PointLatLon::PointLatLon() : m_storage(3, 0.0) {}

/// constructor with lat, lon, and height
PointLatLon::PointLatLon(double lat, double lon, double height) : m_storage(3) {
  m_storage[0] = lat;
  m_storage[1] = lon;
  m_storage[2] = height;
}

/// copy constructor
PointLatLon::PointLatLon(const PointLatLon& other) : m_storage(other.m_storage) {
  m_localCartesianConverter.reset();
}

PointLatLon::~PointLatLon() = default;

/// get lat
double PointLatLon::lat() const {
  return m_storage[0];
}

/// get lon
double PointLatLon::lon() const {
  return m_storage[1];
}

/// get height
double PointLatLon::height() const {
  return m_storage[2];
}

PointLatLon& PointLatLon::operator=(const openstudio::PointLatLon& other) {
  m_storage = other.m_storage;
  m_localCartesianConverter.reset();
  return *this;
}

/// check equality
bool PointLatLon::operator==(const PointLatLon& other) const {
  return (m_storage == other.m_storage);
}

double PointLatLon::operator-(const PointLatLon& other) const {
  double s12;
  const GeographicLib::Geodesic& geod = GeographicLib::Geodesic::WGS84();
  geod.Inverse(lat(), lon(), other.lat(), other.lon(), s12);
  return s12;
}

Point3d PointLatLon::toLocalCartesian(const PointLatLon& point) const {
  initLocalCartesianConverter();
  double x;
  double y;
  double z;
  m_localCartesianConverter->Forward(point.lat(), point.lon(), point.height(), x, y, z);
  return {x, y, z};
}

std::vector<Point3d> PointLatLon::toLocalCartesian(const std::vector<PointLatLon>& points) const {
  std::vector<Point3d> result;
  for (const auto& point : points) {
    result.push_back(toLocalCartesian(point));
  }
  return result;
}

PointLatLon PointLatLon::fromLocalCartesian(const Point3d& point) const {
  initLocalCartesianConverter();
  double lat;
  double lon;
  double h;
  m_localCartesianConverter->Reverse(point.x(), point.y(), point.z(), lat, lon, h);
  return {lat, lon, h};
}

std::vector<PointLatLon> PointLatLon::fromLocalCartesian(const std::vector<Point3d>& points) const {
  std::vector<PointLatLon> result;
  for (const auto& point : points) {
    result.push_back(fromLocalCartesian(point));
  }
  return result;
}

int PointLatLon::utmZone() const {
  return GeographicLib::UTMUPS::StandardZone(lat(), lon());
}

Point3d PointLatLon::toUTM(const PointLatLon& point) const {
  int zone;
  bool northp;
  double x;
  double y;
  double gamma;
  double k;
  GeographicLib::UTMUPS::Forward(point.lat(), point.lon(), zone, northp, x, y, gamma, k, utmZone());
  return {x, y, point.height()};
}

std::vector<Point3d> PointLatLon::toUTM(const std::vector<PointLatLon>& points) const {
  std::vector<Point3d> result;
  for (const auto& point : points) {
    result.push_back(toUTM(point));
  }
  return result;
}

PointLatLon PointLatLon::fromUTM(const Point3d& point) const {
  int zone;
  bool northp;
  {
    double x;
    double y;
    double gamma;
    double k;
    GeographicLib::UTMUPS::Forward(lat(), lon(), zone, northp, x, y, gamma, k);
  }

  double lat;
  double lon;
  double gamma;
  double k;
  GeographicLib::UTMUPS::Reverse(zone, northp, point.x(), point.y(), lat, lon, gamma, k);
  return {lat, lon, point.z()};
}

std::vector<PointLatLon> PointLatLon::fromUTM(const std::vector<Point3d>& points) const {
  std::vector<PointLatLon> result;
  for (const auto& point : points) {
    result.push_back(fromUTM(point));
  }
  return result;
}

void PointLatLon::initLocalCartesianConverter() const {
  if (!m_localCartesianConverter) {
    m_localCartesianConverter = std::unique_ptr<GeographicLib::LocalCartesian>(new GeographicLib::LocalCartesian(lat(), lon(), height()));
  }
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const PointLatLon& point) {
  os << "[" << point.lat() << ", " << point.lon() << ", " << point.height() << "]";
  return os;
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const std::vector<PointLatLon>& pointVector) {
  os << "[";
  for (unsigned i = 0; i < pointVector.size(); ++i) {
    os << pointVector[i];
    if (i < pointVector.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2) {
  PointLatLon p1(lat1, lon1);
  PointLatLon p2(lat2, lon2);
  return (p1 - p2);
}

}  // namespace openstudio

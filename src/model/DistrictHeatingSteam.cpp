/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DistrictHeatingSteam.hpp"
#include "DistrictHeatingSteam_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_DistrictHeating_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    DistrictHeatingSteam_Impl::DistrictHeatingSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DistrictHeatingSteam::iddObjectType());
    }

    DistrictHeatingSteam_Impl::DistrictHeatingSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DistrictHeatingSteam::iddObjectType());
    }

    DistrictHeatingSteam_Impl::DistrictHeatingSteam_Impl(const DistrictHeatingSteam_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DistrictHeatingSteam_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "District Heating Hot Water Rate",    "District Heating Hot Water Energy",   "District Heating Rate",
        "District Heating Inlet Temperature", "District Heating Outlet Temperature", "District Heating Mass Flow Rate"};
      return result;
    }

    IddObjectType DistrictHeatingSteam_Impl::iddObjectType() const {
      return DistrictHeatingSteam::iddObjectType();
    }

    std::vector<ScheduleTypeKey> DistrictHeatingSteam_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_DistrictHeating_SteamFields::CapacityFractionSchedule) != e) {
        result.push_back(ScheduleTypeKey("DistrictHeatingSteam", "Capacity Fraction Schedule"));
      }
      return result;
    }

    boost::optional<double> DistrictHeatingSteam_Impl::nominalCapacity() const {
      return getDouble(OS_DistrictHeating_SteamFields::NominalCapacity, true);
    }

    bool DistrictHeatingSteam_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_DistrictHeating_SteamFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<Schedule> DistrictHeatingSteam_Impl::capacityFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DistrictHeating_SteamFields::CapacityFractionSchedule);
    }

    bool DistrictHeatingSteam_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
      bool result(false);
      if (nominalCapacity) {
        result = setDouble(OS_DistrictHeating_SteamFields::NominalCapacity, nominalCapacity.get());
      }
      return result;
    }

    void DistrictHeatingSteam_Impl::autosizeNominalCapacity() {
      bool result = setString(OS_DistrictHeating_SteamFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    unsigned DistrictHeatingSteam_Impl::inletPort() const {
      return OS_DistrictHeating_SteamFields::SteamInletNodeName;
    }

    unsigned DistrictHeatingSteam_Impl::outletPort() const {
      return OS_DistrictHeating_SteamFields::SteamOutletNodeName;
    }

    bool DistrictHeatingSteam_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    bool DistrictHeatingSteam_Impl::setCapacityFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_DistrictHeating_SteamFields::CapacityFractionSchedule, "DistrictHeatingSteam", "Capacity Fraction", schedule);
      return result;
    }

    void DistrictHeatingSteam_Impl::resetCapacityFractionSchedule() {
      bool result = setString(OS_DistrictHeating_SteamFields::CapacityFractionSchedule, "");
      OS_ASSERT(result);
    }

    boost::optional<double> DistrictHeatingSteam_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void DistrictHeatingSteam_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void DistrictHeatingSteam_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    ComponentType DistrictHeatingSteam_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> DistrictHeatingSteam_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> DistrictHeatingSteam_Impl::heatingFuelTypes() const {
      return {FuelType::DistrictHeatingSteam};
    }

    std::vector<AppGFuelType> DistrictHeatingSteam_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::District};
    }

  }  // namespace detail

  DistrictHeatingSteam::DistrictHeatingSteam(const Model& model) : StraightComponent(DistrictHeatingSteam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DistrictHeatingSteam_Impl>());
    autosizeNominalCapacity();
  }

  IddObjectType DistrictHeatingSteam::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DistrictHeatingSteam);
    return result;
  }

  boost::optional<double> DistrictHeatingSteam::nominalCapacity() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->nominalCapacity();
  }

  bool DistrictHeatingSteam::isNominalCapacityAutosized() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->isNominalCapacityAutosized();
  }

  boost::optional<Schedule> DistrictHeatingSteam::capacityFractionSchedule() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->capacityFractionSchedule();
  }

  bool DistrictHeatingSteam::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void DistrictHeatingSteam::autosizeNominalCapacity() {
    getImpl<detail::DistrictHeatingSteam_Impl>()->autosizeNominalCapacity();
  }

  bool DistrictHeatingSteam::setCapacityFractionSchedule(Schedule& schedule) {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->setCapacityFractionSchedule(schedule);
  }

  void DistrictHeatingSteam::resetCapacityFractionSchedule() {
    getImpl<detail::DistrictHeatingSteam_Impl>()->resetCapacityFractionSchedule();
  }

  /// @cond

  DistrictHeatingSteam::DistrictHeatingSteam(std::shared_ptr<detail::DistrictHeatingSteam_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> DistrictHeatingSteam::autosizedNominalCapacity() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model

}  // namespace openstudio

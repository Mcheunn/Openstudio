/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_ZoneCapacitanceMultiplier_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneCapacitanceMultiplierResearchSpecial_Impl::ZoneCapacitanceMultiplierResearchSpecial_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
    }

    ZoneCapacitanceMultiplierResearchSpecial_Impl::ZoneCapacitanceMultiplierResearchSpecial_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
    }

    ZoneCapacitanceMultiplierResearchSpecial_Impl::ZoneCapacitanceMultiplierResearchSpecial_Impl(
      const ZoneCapacitanceMultiplierResearchSpecial_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneCapacitanceMultiplierResearchSpecial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ZoneCapacitanceMultiplierResearchSpecial_Impl::iddObjectType() const {
      return ZoneCapacitanceMultiplierResearchSpecial::iddObjectType();
    }

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ZoneCapacitanceMultiplierResearchSpecial_Impl::parent() const {
      boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
      return result;
    }

    // set the parent, child may have to call methods on the parent
    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setParent(ParentObject& newParent) {
      if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
        return true;
      }
      return false;
    }

    double ZoneCapacitanceMultiplierResearchSpecial_Impl::temperatureCapacityMultiplier() const {
      boost::optional<double> value = getDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::isTemperatureCapacityMultiplierDefaulted() const {
      return isEmpty(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier);
    }

    double ZoneCapacitanceMultiplierResearchSpecial_Impl::humidityCapacityMultiplier() const {
      boost::optional<double> value = getDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::isHumidityCapacityMultiplierDefaulted() const {
      return isEmpty(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier);
    }

    double ZoneCapacitanceMultiplierResearchSpecial_Impl::carbonDioxideCapacityMultiplier() const {
      boost::optional<double> value = getDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::isCarbonDioxideCapacityMultiplierDefaulted() const {
      return isEmpty(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier);
    }

    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier) {
      bool result = setDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier, temperatureCapacityMultiplier);
      return result;
    }

    void ZoneCapacitanceMultiplierResearchSpecial_Impl::resetTemperatureCapacityMultiplier() {
      bool result = setString(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier, "");
      OS_ASSERT(result);
    }

    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setHumidityCapacityMultiplier(double humidityCapacityMultiplier) {
      bool result = setDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, humidityCapacityMultiplier);
      return result;
    }

    void ZoneCapacitanceMultiplierResearchSpecial_Impl::resetHumidityCapacityMultiplier() {
      bool result = setString(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, "");
      OS_ASSERT(result);
    }

    bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier) {
      bool result = setDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier, carbonDioxideCapacityMultiplier);
      return result;
    }

    void ZoneCapacitanceMultiplierResearchSpecial_Impl::resetCarbonDioxideCapacityMultiplier() {
      bool result = setString(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType ZoneCapacitanceMultiplierResearchSpecial::iddObjectType() {
    return {IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial};
  }

  double ZoneCapacitanceMultiplierResearchSpecial::temperatureCapacityMultiplier() const {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->temperatureCapacityMultiplier();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial::isTemperatureCapacityMultiplierDefaulted() const {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->isTemperatureCapacityMultiplierDefaulted();
  }

  double ZoneCapacitanceMultiplierResearchSpecial::humidityCapacityMultiplier() const {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->humidityCapacityMultiplier();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial::isHumidityCapacityMultiplierDefaulted() const {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->isHumidityCapacityMultiplierDefaulted();
  }

  double ZoneCapacitanceMultiplierResearchSpecial::carbonDioxideCapacityMultiplier() const {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->carbonDioxideCapacityMultiplier();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial::isCarbonDioxideCapacityMultiplierDefaulted() const {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->isCarbonDioxideCapacityMultiplierDefaulted();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial::setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier) {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setTemperatureCapacityMultiplier(temperatureCapacityMultiplier);
  }

  void ZoneCapacitanceMultiplierResearchSpecial::resetTemperatureCapacityMultiplier() {
    getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->resetTemperatureCapacityMultiplier();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial::setHumidityCapacityMultiplier(double humidityCapacityMultiplier) {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setHumidityCapacityMultiplier(humidityCapacityMultiplier);
  }

  void ZoneCapacitanceMultiplierResearchSpecial::resetHumidityCapacityMultiplier() {
    getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->resetHumidityCapacityMultiplier();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial::setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier) {
    return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setCarbonDioxideCapacityMultiplier(carbonDioxideCapacityMultiplier);
  }

  void ZoneCapacitanceMultiplierResearchSpecial::resetCarbonDioxideCapacityMultiplier() {
    getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->resetCarbonDioxideCapacityMultiplier();
  }

  /// @cond
  ZoneCapacitanceMultiplierResearchSpecial::ZoneCapacitanceMultiplierResearchSpecial(
    std::shared_ptr<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl> impl)
    : ModelObject(std::move(impl)) {}
  ZoneCapacitanceMultiplierResearchSpecial::ZoneCapacitanceMultiplierResearchSpecial(Model& model)
    : ModelObject(ZoneCapacitanceMultiplierResearchSpecial::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>());
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio

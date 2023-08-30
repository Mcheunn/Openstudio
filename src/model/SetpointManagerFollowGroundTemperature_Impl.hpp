/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_IMPL_HPP
#define MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerFollowGroundTemperature_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerFollowGroundTemperature.*/
    class MODEL_API SetpointManagerFollowGroundTemperature_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerFollowGroundTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerFollowGroundTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerFollowGroundTemperature_Impl(const SetpointManagerFollowGroundTemperature_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerFollowGroundTemperature_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // virtual bool addToNode(Node & node) override;

      virtual bool isAllowedOnPlantLoop() const override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      std::string referenceGroundTemperatureObjectType() const;

      double offsetTemperatureDifference() const;

      double maximumSetpointTemperature() const;

      double minimumSetpointTemperature() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType);

      bool setOffsetTemperatureDifference(double offsetTemperatureDifference);

      bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

      bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerFollowGroundTemperature");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_IMPL_HPP

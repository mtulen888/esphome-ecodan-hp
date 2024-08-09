#pragma once
#include <mutex>
#include <string>

namespace esphome {
namespace ecodan 
{    

    struct Status
    {
        bool Initialized = false;

        bool DefrostActive;
        bool DhwForcedActive;        
        bool BoosterActive;
        bool ImmersionActive;
        uint8_t HeatSource;
        bool In1ThermostatRequest;
        bool In5ThermostatRequest;
        bool In6ThermostatRequest;
        uint8_t OutputPower;
        float ComputedOutputPower;
        uint8_t Controller;

        float Zone1SetTemperature{NAN};
        float Zone1FlowTemperatureSetPoint{NAN};
        float Zone1RoomTemperature{NAN};
        float Zone2SetTemperature{NAN};
        float Zone2FlowTemperatureSetPoint{NAN};
        float Zone2RoomTemperature{NAN};
        float LegionellaPreventionSetPoint;
        float DhwTemperatureDrop;
        uint8_t MaximumFlowTemperature;
        uint8_t MinimumFlowTemperature;
        float OutsideTemperature;
        float HpFeedTemperature;
        float HpReturnTemperature;
        float HpRefrigerantLiquidTemperature;
        float HpRefrigerantCondensingTemperature;
        float DhwTemperature{NAN};
        float DhwSecondaryTemperature;
        float BoilerFlowTemperature;
        float BoilerReturnTemperature;
        uint8_t FlowRate;
        float DhwFlowTemperatureSetPoint{NAN};
        float RadiatorFlowTemperatureSetPoint;

        float Runtime;
        bool WaterPumpActive;
        bool WaterPump2Active;
        bool ThreeWayValveActive;
        bool ThreeWayValve2Active;

        enum class PowerMode : uint8_t
        {
            STANDBY = 0,
            ON = 1
        };

        enum class OperationMode : uint8_t
        {
            OFF = 0,
            DHW_ON = 1,
            HEAT_ON = 2, // Heating
            COOL_ON = 3, // Cooling
            FROST_PROTECT = 5,
            LEGIONELLA_PREVENTION = 6
        };

        enum class DhwMode : uint8_t
        {
            NORMAL = 0,
            ECO = 1
        };

        enum class HpMode : uint8_t
        {
            OFF = 255,
            HEAT_ROOM_TEMP = 0,
            HEAT_FLOW_TEMP = 1,
            HEAT_COMPENSATION_CURVE = 2,
            COOL_ROOM_TEMP = 3,
            COOL_FLOW_TEMP = 4
        };

        // Modes
        PowerMode Power;
        OperationMode Operation;
        bool HolidayMode;
        DhwMode HotWaterMode;
        HpMode HeatingCoolingMode = HpMode::OFF;

        // prohibit flags
        bool ProhibitDhw;
        bool ProhibitHeatingZ1;
        bool ProhibitCoolingZ1;
        bool ProhibitHeatingZ2;
        bool ProhibitCoolingZ2;

        // Efficiency
        uint8_t CompressorFrequency;
        float EnergyConsumedHeating;
        float EnergyConsumedCooling;
        float EnergyDeliveredHeating;
        float EnergyDeliveredCooling;
        float EnergyConsumedDhw;
        float EnergyDeliveredDhw;

        void update_output_power_estimation() {
            if (CompressorFrequency > 0 && FlowRate > 0)  {
                ComputedOutputPower = FlowRate/60.0 * abs(HpFeedTemperature - HpReturnTemperature) * 4.18f;
            }
            else {
                ComputedOutputPower = 0.0f;
            }
        }

        std::string power_as_string()
        {
            switch (Power)
            {
                case PowerMode::ON:
                    return std::string("On");
                case PowerMode::STANDBY:
                    return std::string("Standby");
                default:
                    return std::string("Unknown");
            }
        }

        std::string operation_as_string()
        {
            switch (Operation)
            {
                case OperationMode::OFF:
                    return std::string("Off");
                case OperationMode::DHW_ON:
                    return std::string("Heating Water");
                case OperationMode::HEAT_ON:
                    return std::string("Space Heating");
                case OperationMode::COOL_ON:
                    return std::string("Space Cooling");
                case OperationMode::FROST_PROTECT:
                    return std::string("Frost Protection");
                case OperationMode::LEGIONELLA_PREVENTION:
                    return std::string("Legionella Prevention");
                default:
                    return std::string("Unknown");
            }
        }

        std::string dhw_status_as_string()
        {
            // switch (Operation)
            // {
            //     case OperationMode::DHW_ON:
            //     case OperationMode::LEGIONELLA_PREVENTION:
            //         break;
            //     default:
            //         return std::string("Off");
            // }

            switch (HotWaterMode)
            {
                case DhwMode::NORMAL:
                    return std::string("Normal");
                case DhwMode::ECO:
                    return std::string("Eco");
                default:
                    return std::string("Unknown");
            }
        }

        std::string hp_status_as_string()
        {
            switch (HeatingCoolingMode)
            {
                case HpMode::HEAT_ROOM_TEMP:
                    return std::string("Heat Target Temperature");
                case HpMode::HEAT_FLOW_TEMP:
                    return std::string("Heat Flow Temperature");
                case HpMode::HEAT_COMPENSATION_CURVE:
                    return std::string("Heat Compensation Curve");
                case HpMode::COOL_ROOM_TEMP:
                    return std::string("Cool Target Temperature");
                case HpMode::COOL_FLOW_TEMP:
                    return std::string("Cool Flow Temperature");
                default:
                    return std::string("Unknown");
            }
        }

        void set_power_mode(uint8_t mode)
        {
            Power = static_cast<PowerMode>(mode);
        }

        void set_operation_mode(uint8_t mode)
        {
            Operation = static_cast<OperationMode>(mode);
        }

        void set_dhw_mode_status(uint8_t mode)
        {
            HotWaterMode = static_cast<DhwMode>(mode);
        }

        void set_heating_cooling_mode(uint8_t mode)
        {
            HeatingCoolingMode = static_cast<HpMode>(mode);
        }
    };

} // namespace ecodan
} // namespace esphome
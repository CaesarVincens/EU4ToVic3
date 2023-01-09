#include "outBuildings.h"
#include "ClayManager/State/SubState.h"
#include "CommonFunctions.h"
#include "PoliticalManager/Country/Country.h"
#include <fstream>
#include <ranges>

namespace
{
void outPMs(std::ostream& output, const std::string& building, const V3::Country& country)
{
	output << "\t\t\t\tactivate_production_methods = {";
	if (country.getProcessedData().productionMethods.contains(building))
	{
		for (const auto& PM: country.getProcessedData().productionMethods.at(building))
		{
			output << " \"" << PM << "\"";
		}
	}
	output << " }\n";
}
void outBuilding(std::ostream& output, const std::string& building, const int level, const V3::Country& country)
{
	output << "\t\t\tcreate_building = {\n";
	output << "\t\t\t\tbuilding = \"" << building << "\"\n";
	output << "\t\t\t\tlevel = " << level << "\n";
	output << "\t\t\t\treserves = " << 1 << "\n";

	outPMs(output, building, country);

	output << "\t\t\t}\n";
}
void outSubStateBuildings(std::ostream& output, const V3::SubState& subState)
{
	if (!subState.getOwner())
		return;
	output << "\t\tregion_state:" << subState.getOwner()->getTag() << " = {\n";
	for (const auto& [building, level]: subState.getBuildings())
	{
		if (level > 0)
			outBuilding(output, building, level, *subState.getOwner());
	}
	output << "\t\t}\n";
}
void outStateBuildings(std::ostream& output, const V3::State& state)
{
	output << "\ts:" << state.getName() << " = {\n";
	for (const auto& substate: state.getSubStates())
	{
		if (substate->getBuildings().empty())
			continue;
		outSubStateBuildings(output, *substate);
	}
	output << "\t}\n";
}
} // namespace
void OUT::exportBuildings(const std::string& outputName, const std::map<std::string, std::shared_ptr<V3::State>>& states)
{
	std::ofstream output("output/" + outputName + "/common/history/buildings/99_converted_buildings.txt");
	if (!output.is_open())
		throw std::runtime_error("Could not create " + outputName + "/common/history/buildings/99_converted_buildings.txt");

	output << commonItems::utf8BOM << "BUILDINGS = {\n";
	for (const auto& state: states | std::views::values)
	{
		if (std::ranges::all_of(state->getSubStates(), [](const auto& subState) {
				 return subState->getBuildings().empty();
			 }))
		{
			continue;
		}

		outStateBuildings(output, *state);
	}
	output << "}\n";
	output.close();
}

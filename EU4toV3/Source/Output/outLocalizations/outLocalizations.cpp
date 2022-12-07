#include "outLocalizations.h"
#include "CommonFunctions.h"
#include <fstream>
#include <ranges>

void OUT::exportCountryNamesAndAdjectives(const std::string& outputName, const std::map<std::string, std::shared_ptr<V3::Country>>& countries)
{
	const std::set<std::string> knownVic3Localizations =
		 {"braz_por", "english", "french", "german", "japanese", "korean", "polish", "russian", "simp_chinese", "spanish", "turkish"};

	for (const auto& language: knownVic3Localizations)
	{
		std::ofstream output("output/" + outputName + "/localization/" + language + "/converted_countries_l_" + language + ".yml");
		if (!output.is_open())
			throw std::runtime_error("output/" + outputName + "/localization/" + language + "/converted_countries_l_" + language + ".yml");

		output << commonItems::utf8BOM << "l_" << language << ":\n";
		for (const auto& country: countries | std::views::values)
		{
			output << " " << country->getTag() << ": \"" << country->getName(language) << "\"\n";
			output << " " << country->getTag() << "_ADJ: \"" << country->getAdjective(language) << "\"\n";
		}
		output.close();
	}
}

void OUT::exportReligionLocs(const std::string& outputName, const std::map<std::string, mappers::ReligionDef>& religions)
{
	const std::set<std::string> knownVic3Localizations =
		 {"braz_por", "english", "french", "german", "japanese", "korean", "polish", "russian", "simp_chinese", "spanish", "turkish"};

	for (const auto& language: knownVic3Localizations)
	{
		std::ofstream output("output/" + outputName + "/localization/" + language + "/99_converted_religions_l_" + language + ".yml");
		if (!output.is_open())
			throw std::runtime_error("output/" + outputName + "/localization/" + language + "/99_converted_religions_l_" + language + ".yml");

		output << commonItems::utf8BOM << "l_" << language << ":\n";
		for (const auto& religion: religions | std::views::values)
		{
			if (religion.locBlock.contains(language))
				output << " " << religion.name << ": \"" << religion.locBlock.at(language) << "\"\n";
			else if (religion.locBlock.contains("english"))
				output << " " << religion.name << ": \"" << religion.locBlock.at("english") << "\"\n";
		}
		output.close();
	}
}

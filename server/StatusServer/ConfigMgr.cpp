#include "ConfigMgr.h"
// 用boost管理读文件,windows和linux都能用
ConfigMgr::ConfigMgr()
{
	boost::filesystem::path current_path = boost::filesystem::current_path();
	// 拼接, 重载了/运算符
	boost::filesystem::path config_path = current_path / "config.ini";
	std::cout << "Config path : " << config_path << std::endl;

	// 桉树的形式读文件
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	// 遍历section
	for (const auto& section_pair : pt) {
		const std::string& section_name = section_pair.first;
		const boost::property_tree::ptree& section_tree = section_pair.second;
		std::map<std::string, std::string> section_config;
		// section里的key-value对
		for (const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			// second还是一个tree,因此要转为string类型
			const std::string& value = key_value_pair.second.get_value<std::string>();
			section_config[key] = value;
		}

		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config;
		_config_map[section_name] = sectionInfo;
	}

	// 输出所有的section和key-value对
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		SectionInfo section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}
}

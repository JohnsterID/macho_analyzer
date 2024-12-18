// macho_parser.h
#pragma once

#include <LIEF/LIEF.hpp>
#include <LIEF/MachO.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <string>
#include <memory>
#include <vector>
#include "macho_symbol.h"
#include "macho_section.h"
#include "macho_segment.h"
#include <set>
#include <chrono>

class MachoParser {
public:
    // Error reporting
    struct ParseResult {
        bool success;
        std::string error_message;
        std::vector<std::string> warnings;
    };
	
    struct BinaryInfo {
        uint64_t total_size;
        uint64_t code_size;
        uint64_t data_size;
        uint64_t linkedit_size;
        bool has_debug_info;
        std::vector<std::string> section_types;
        size_t symbol_count;
        size_t export_count;
        size_t import_count;
    };

    // Static factory method to create parser
    static std::unique_ptr<MachoParser> parse(const std::string& filepath, ParseResult& result);

    // Delete copy/move constructors and assignments
    MachoParser(const MachoParser&) = delete;
    MachoParser& operator=(const MachoParser&) = delete;
    MachoParser(MachoParser&&) = delete;
    MachoParser& operator=(MachoParser&&) = delete;

    virtual ~MachoParser() = default;

    nlohmann::json to_json() const;

    // Getters
    const std::vector<std::unique_ptr<MachoSymbol>>& get_symbols() const { return symbols_; }
    const std::vector<std::unique_ptr<MachoSection>>& get_sections() const { return sections_; }
    const std::vector<std::unique_ptr<MachoSegment>>& get_segments() const { return segments_; }

    std::string get_architecture() const;
    std::string get_file_type() const { return ""; }
    uint32_t get_flags() const { return 0; }
    bool is_64_bit() const { return false; }

    std::vector<std::string> get_exported_symbols() const { return {}; }
    std::vector<std::string> get_imported_symbols() const { return {}; }

    const std::vector<std::string>& get_warnings() const { return warnings_; }
	BinaryInfo analyze_binary() const;

protected:
    // Allow construction, but keep it protected
    MachoParser() = default;

private:
    std::vector<std::string> warnings_;
    bool init(const std::string& filepath);
    void parse_symbols();
    void parse_sections();
    void parse_segments();
    void parse_dyld_info();

    std::unique_ptr<LIEF::MachO::FatBinary> binary_;
    std::vector<std::unique_ptr<MachoSymbol>> symbols_;
    std::vector<std::unique_ptr<MachoSection>> sections_;
    std::vector<std::unique_ptr<MachoSegment>> segments_;
};
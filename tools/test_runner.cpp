#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct CaseResult {
    std::string id;
    bool pass;
    std::string details;
};

struct ParsedLog {
    int dataCount = 0;
    int metricCount = 0;
    int faultCount = 0;
    int eventBootCount = 0;
    bool hasRms = false;
    bool hasSat = false;
};

static std::string trim(const std::string&amp; s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static ParsedLog parseLog(const std::string&amp; path) {
    ParsedLog p;
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.rfind("DATA,", 0) == 0) p.dataCount++;
        if (line.rfind("METRIC,", 0) == 0) {
            p.metricCount++;
            if (line.find("rms_cm") != std::string::npos) p.hasRms = true;
            if (line.find("sat_pct") != std::string::npos) p.hasSat = true;
        }
        if (line.rfind("FAULT,", 0) == 0) p.faultCount++;
        if (line.find("EVENT,BOOT,ok") != std::string::npos) p.eventBootCount++;
    }
    return p;
}

static CaseResult evalCP01(const ParsedLog&amp; p) {
    bool ok = (p.eventBootCount > 0) &amp;&amp; (p.dataCount > 5) &amp;&amp; (p.metricCount > 0);
    return {"CP-01", ok, "BOOT=" + std::to_string(p.eventBootCount) + ", DATA=" + std::to_string(p.dataCount) + ", METRIC=" + std::to_string(p.metricCount)};
}

static CaseResult evalCP03(const ParsedLog&amp; p) {
    bool ok = (p.dataCount > 0) &amp;&amp; (p.metricCount > 0) &amp;&amp; p.hasRms &amp;&amp; p.hasSat;
    return {"CP-03", ok, "DATA=" + std::to_string(p.dataCount) + ", METRIC=" + std::to_string(p.metricCount) + ", hasRms=" + std::string(p.hasRms ? "true" : "false") + ", hasSat=" + std::string(p.hasSat ? "true" : "false")};
}

static CaseResult evalEX03(const ParsedLog&amp; p) {
    bool ok = p.faultCount > 0;
    return {"EX-03", ok, "FAULT=" + std::to_string(p.faultCount)};
}

static void printUsage() {
    std::cout << "Uso:\n";
    std::cout << "  test_runner.exe --scenario CP-01 --log docs/evidence/cp01.log\n";
    std::cout << "  test_runner.exe --scenario CP-03 --log docs/evidence/cp03.log\n";
    std::cout << "  test_runner.exe --scenario EX-03 --log docs/evidence/ex03.log\n";
}

int main(int argc, char** argv) {
    std::string scenario;
    std::string logPath;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--scenario" &amp;&amp; i + 1 < argc) {
            scenario = argv[++i];
        } else if (arg == "--log" &amp;&amp; i + 1 < argc) {
            logPath = argv[++i];
        }
    }

    if (scenario.empty() || logPath.empty()) {
        printUsage();
        return 1;
    }

    std::ifstream check(logPath);
    if (!check.good()) {
        std::cerr << "No se pudo abrir log: " << logPath << "\n";
        return 2;
    }

    ParsedLog parsed = parseLog(logPath);
    CaseResult result;

    if (scenario == "CP-01") {
        result = evalCP01(parsed);
    } else if (scenario == "CP-03") {
        result = evalCP03(parsed);
    } else if (scenario == "EX-03") {
        result = evalEX03(parsed);
    } else {
        std::cerr << "Escenario no soportado por ahora: " << scenario << "\n";
        std::cerr << "Soportados: CP-01, CP-03, EX-03\n";
        return 3;
    }

    std::cout << "SCENARIO=" << result.id << "\n";
    std::cout << "RESULT=" << (result.pass ? "PASS" : "FAIL") << "\n";
    std::cout << "DETAILS=" << result.details << "\n";

    return result.pass ? 0 : 10;
}

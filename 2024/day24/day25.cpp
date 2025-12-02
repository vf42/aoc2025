#include <fstream>
#include <array>
#include <print>    // C++23 std::print
#include <expected> // C++23 std::expected
#include <ranges>
#include <generator>
#include <numeric>

struct Item {
    std::array<uint8_t, 5> heights;
    bool is_lock;
};

std::generator<Item> parse_items(std::ifstream& input) {
    std::string line;
    while (std::getline(input, line)) {
        Item item{};
        bool first_line = true;

        do {
            for (auto const [i, c] : std::ranges::views::enumerate(line)) {
                if (c == '#') {
                    item.heights[i] += 1;
                }
            }
            if (first_line) {
                first_line = false;
                item.is_lock = std::accumulate(
                    item.heights.begin(), item.heights.end(), 0
                ) == 5;
            }
        } while (std::getline(input, line) && !line.empty());
        co_yield item;
    }
}

std::expected<void, std::string> process_file(std::string_view fname) {
    std::ifstream file(fname.data());
    if (!file) {
        return std::unexpected(
            std::format("Unknown file: {}", fname)
        );
    }
    for (const auto& item : parse_items(file)) {
        std::print("Parsed item: lock={}, heights=[", item.is_lock);
        for (size_t i = 0; i < item.heights.size(); ++i) {
            std::print("{}", static_cast<int>(item.heights[i]));
            if (i < item.heights.size() - 1) std::print(", ");
        }
        std::println("]");
    }

    return {};
}

int main(int argc, char* argv[]) {
    std::span args(argv, static_cast<size_t>(argc));
    std::string_view fname  = args.size() > 1 ? args[1] : "test.txt";

    if (auto result = process_file(fname); !result) {
        std::println(stderr, "Error: {}", result.error());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

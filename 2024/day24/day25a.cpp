#include <fstream>
#include <array>
#include <print>    // C++23 std::print
#include <expected> // C++23 std::expected
#include <ranges>
#include <generator>
#include <numeric>
#include <vector>
#include <algorithm>

// Index 0 determines whether it's lock.
using Item = std::array<int8_t, 6>;

std::generator<Item> parse_items(std::ifstream& input) {
    std::string line;
    while (std::getline(input, line)) {
        Item item = {0, -1, -1, -1, -1, -1};
        bool first_line = true;

        do {
            for (auto const [i, c] : std::views::enumerate(line)) {
                if (c == '#') {
                    item[i+1] += 1;
                }
            }
            if (first_line) {
                first_line = false;
                item[0] = std::accumulate(
                    item.begin(), item.end(), 0
                ) == 0;
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
    const std::vector<Item> items = std::ranges::to<std::vector>(
        parse_items(file));
    int fitting_pairs = 0;
    for (const auto [i, a] : std::views::enumerate(items)) {
        for (const auto& b: items | std::views::drop(i + 1)) {
            // Only match lock against locks.
            if ((a[0] ^ b[0]) == 0)
                continue;
            auto ah = a | std::views::drop(1);
            auto bh = b | std::views::drop(1);
            auto tmp = std::views::zip(ah, bh)
                | std::views::transform([](auto val) {
                    return (std::get<0>(val) + std::get<1>(val)) >= 6;
                });
            auto result = std::accumulate(
                tmp.begin(), tmp.end(), 0
            );
            if (result == 0)
                ++fitting_pairs;
        }
    }
    std::println("Answer: {}", fitting_pairs);

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

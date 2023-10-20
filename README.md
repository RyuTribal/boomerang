## Boomerang

**Description:** A final exam at the university. I got some messy code and they told me to refactor it based on modularity and extendability

### Prerequisites
- This project is developed using Visual Studio with the C++ extension enabled.

### Setup Instructions

1. Execute `generate_projects.bat`.
2. When prompted, select your Visual Studio version (for instance, vs2022).
3. Open the generated project in Visual Studio.
4. Compile the project from within the Visual Studio environment.

### Running Tests
- To execute the tests, simply run `run_tests.bat`.

### Adding New Tests
- If you wish to include additional tests, create a new `.cpp` file inside the `tests` directory.

Here is a test example:

```
#include <gtest/gtest.h>

TEST_F(GameTest, DealCards) {
    
    Boomerang::Boomerang game{ test_deck.get(), test_variant.get()};
    for (auto &player : game.variant()->GetPlayers()) {
        EXPECT_EQ(player.second->GetHand()->size(), 7); // each player should have 7 cards
    }
}
```

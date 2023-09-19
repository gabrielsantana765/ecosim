#define CROW_MAIN
#define CROW_STATIC_DIR "../public"

#include "crow_all.h"
#include "json.hpp"
#include <random>

// Function to generate a random action based on probability
bool random_action(float probability) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < probability;
}

// Simulate random actions for different entities
void simulate_random_actions() {
    // Probabilities for different actions
    float plant_growth_probability = 0.20; // 20% chance of growth
    float herbivore_move_probability = 0.70; // 70% chance to action
    float carnivore_move_probability = 0.60; // 60% chance to action

    // Simulate plant growth
    if (random_action(plant_growth_probability)) {
        std::cout << "Plant grows.\n";
    } else {
        std::cout << "Plant does not grow.\n";
    }

    // Simulate herbivore action
    if (random_action(herbivore_move_probability)) {
        std::cout << "Herbivore moves.\n";
    } else {
        std::cout << "Herbivore does not move.\n";
    }

    // Simulate carnivore action
    if (random_action(carnivore_move_probability)) {
        std::cout << "Carnivore moves.\n";
    } else {
        std::cout << "Carnivore does not move.\n";
    }
}
static const uint32_t NUM_ROWS = 15;

// Constants
const uint32_t PLANT_MAXIMUM_AGE = 10;
const uint32_t HERBIVORE_MAXIMUM_AGE = 50;
const uint32_t CARNIVORE_MAXIMUM_AGE = 80;
const uint32_t MAXIMUM_ENERGY = 200;
const uint32_t THRESHOLD_ENERGY_FOR_REPRODUCTION = 20;

// Probabilities
const double PLANT_REPRODUCTION_PROBABILITY = 0.20;
const double HERBIVORE_REPRODUCTION_PROBABILITY = 0.075;
const double CARNIVORE_REPRODUCTION_PROBABILITY = 0.025;
const double HERBIVORE_MOVE_PROBABILITY = 0.7;
const double HERBIVORE_EAT_PROBABILITY = 0.9;
const double CARNIVORE_MOVE_PROBABILITY = 0.5;
const double CARNIVORE_EAT_PROBABILITY = 1.0;

// Type definitions
enum entity_type_t
{
    empty,
    plant,
    herbivore,
    carnivore
};

struct pos_t
{
    uint32_t i;
    uint32_t j;
};

struct entity_t
{
    entity_type_t type;
    int32_t energy;
    int32_t age;
};

// Auxiliary code to convert the entity_type_t enum to a string
NLOHMANN_JSON_SERIALIZE_ENUM(entity_type_t, {
                                                {empty, " "},
                                                {plant, "P"},
                                                {herbivore, "H"},
                                                {carnivore, "C"},
                                            })

// Auxiliary code to convert the entity_t struct to a JSON object
namespace nlohmann
{
    void to_json(nlohmann::json &j, const entity_t &e)
    {
        j = nlohmann::json{{"type", e.type}, {"energy", e.energy}, {"age", e.age}};
    }
}

// Grid that contains the entities
static std::vector<std::vector<entity_t>> entity_grid;

int main()
{
    crow::SimpleApp app;
    simulate_random_actions();
    // Endpoint to serve the HTML page
    CROW_ROUTE(app, "/")
    ([](crow::request &, crow::response &res)
     {
        // Return the HTML content here
        res.set_static_file_info_unsafe("../public/index.html");
        res.end(); });

    CROW_ROUTE(app, "/start-simulation")
        .methods("POST"_method)([](crow::request &req, crow::response &res)
                                { 
        // Parse the JSON request body
        nlohmann::json request_body = nlohmann::json::parse(req.body);

       // Validate the request body 
        uint32_t total_entinties = (uint32_t)request_body["plants"] + (uint32_t)request_body["herbivores"] + (uint32_t)request_body["carnivores"];
        if (total_entinties > NUM_ROWS * NUM_ROWS) {
        res.code = 400;
        res.body = "Too many entities";
        res.end();
        return;
        }

        // Clear the entity grid
        entity_grid.clear();
        entity_grid.assign(NUM_ROWS, std::vector<entity_t>(NUM_ROWS, { empty, 0, 0}));
        
        // Create the entities
        // <YOUR CODE HERE>
        // Create the entities
        // Get the number of plants, herbivores, and carnivores from the request body
    
        uint32_t numPlants = (uint32_t)request_body["plants"];
        uint32_t numHerbivores = (uint32_t)request_body["herbivores"];
        uint32_t numCarnivores = (uint32_t)request_body["carnivores"];

        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> randRow(0, NUM_ROWS - 1);
        std::uniform_int_distribution<int> randCol(0, NUM_ROWS - 1);

        // Create and place plants
        for (uint32_t i = 0; i < numPlants; ++i) {
            int row, col;
            do {
                row = randRow(gen);
                col = randCol(gen);
            } while (entity_grid[row][col].type != empty);
            
            entity_grid[row][col] = {plant, MAXIMUM_ENERGY, 0};
        }

        // Create and place herbivores
        for (uint32_t i = 0; i < numHerbivores; ++i) {
            int row, col;
            do {
                row = randRow(gen);
                col = randCol(gen);
            } while (entity_grid[row][col].type != empty);
            
            entity_grid[row][col] = {herbivore, MAXIMUM_ENERGY, 0};
        }

        // Create and place carnivores
        for (uint32_t i = 0; i < numCarnivores; ++i) {
            int row, col;
            do {
                row = randRow(gen);
                col = randCol(gen);
            } while (entity_grid[row][col].type != empty);
            
            entity_grid[row][col] = {carnivore, MAXIMUM_ENERGY, 0};
        }
//fim


        // Return the JSON representation of the entity grid
        nlohmann::json json_grid = entity_grid; 
        res.body = json_grid.dump();
        res.end(); });

    // Endpoint to process HTTP GET requests for the next simulation iteration
    CROW_ROUTE(app, "/next-iteration")
        .methods("GET"_method)([]()
                               {
        // Simulate the next iteration
        // Iterate over the entity grid and simulate the behaviour of each entity
        
        // <YOUR CODE HERE>
        // Simulate the next iteration
        for (uint32_t row = 0; row < NUM_ROWS; ++row) {
            for (uint32_t col = 0; col < NUM_ROWS; ++col) {
                entity_t &currentEntity = entity_grid[row][col];
    float herbivore_move_probability = 0.70; // 70% chance to action
    float carnivore_move_probability = 0.60; // 60% chance to action
                if (currentEntity.type == plant) {
                    // Regras para plantas
                    if (currentEntity.age >= PLANT_MAXIMUM_AGE) {
                        // Planta morre após 10 etapas de tempo
                        currentEntity = {empty, 0, 0};
                    } else {
                        // Simulate plant growth using the random_action function
                        if (random_action(PLANT_REPRODUCTION_PROBABILITY)) {
                            // Encontre uma célula vazia adjacente aleatoriamente
                            int newRow, newCol;
                            do {
                                int direction = rand() % 4; // 0: cima, 1: baixo, 2: esquerda, 3: direita
                                newRow = row;
                                newCol = col;
                                if (direction == 0) {
                                    newRow--;
                                } else if (direction == 1) {
                                    newRow++;
                                } else if (direction == 2) {
                                    newCol--;
                                } else {
                                    newCol++;
                                }
                            } while (newRow < 0 || newRow >= NUM_ROWS || newCol < 0 || newCol >= NUM_ROWS || entity_grid[newRow][newCol].type != empty);
                            
                            // Crie uma nova planta na célula vazia adjacente
                            entity_grid[newRow][newCol] = {plant, MAXIMUM_ENERGY, 0};
                        }
                        // Atualize a idade da planta
                        currentEntity.age++;
                    }
                } else if (currentEntity.type == herbivore) {
                    // Regras para herbívoros
    if (currentEntity.age >= HERBIVORE_MAXIMUM_AGE || currentEntity.energy <= 0) {
        // Herbívoro morre após 50 etapas de tempo ou se a energia chegar a 0
        currentEntity = {empty, 0, 0};
    } else {
        // Simulate herbivore move based on the herbivore_move_probability
        if (random_action(herbivore_move_probability)) {
            // Encontre uma célula adjacente aleatória (excluindo células com carnívoros)
            int newRow, newCol;
            bool foundEmptyCell = false;
            int attempts = 0;
            while (!foundEmptyCell && attempts < 4) {
                int direction = rand() % 4; // 0: cima, 1: baixo, 2: esquerda, 3: direita
                newRow = row;
                newCol = col;
                if (direction == 0) {
                    newRow--;
                } else if (direction == 1) {
                    newRow++;
                } else if (direction == 2) {
                    newCol--;
                } else {
                    newCol++;
                }
                
                // Verifique se a célula é válida e está vazia (não contém carnívoros)
                if (newRow >= 0 && newRow < NUM_ROWS && newCol >= 0 && newCol < NUM_ROWS && entity_grid[newRow][newCol].type == empty) {
                    foundEmptyCell = true;
                }
                
                attempts++;
            }
            
            if (foundEmptyCell) {
                // Movimente o herbívoro para a célula adjacente
                entity_grid[newRow][newCol] = currentEntity;
                entity_grid[row][col] = {empty, 0, 0};
                currentEntity = {herbivore, currentEntity.energy - 5, 0}; // A energia é gasta no movimento
            }
        } else {
            // O herbívoro não se move nesta iteração, mas gasta energia
            currentEntity.energy -= 5;
        }
        
        // Verifique a probabilidade de alimentação
        double eatProbability = (double)rand() / RAND_MAX;
        if (eatProbability <= HERBIVORE_EAT_PROBABILITY) {
            // Verifique se há uma planta adjacente para comer
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int adjRow = row + i;
                    int adjCol = col + j;
                    if (adjRow >= 0 && adjRow < NUM_ROWS && adjCol >= 0 && adjCol < NUM_ROWS &&
                        entity_grid[adjRow][adjCol].type == plant) {
                        // Herbívoro come a planta
                        currentEntity.energy += 30;
                        // Remova a planta
                        entity_grid[adjRow][adjCol] = {empty, 0, 0};
                    }
                }
            }
        }
        
        // Verifique a probabilidade de reprodução
        if (currentEntity.energy > THRESHOLD_ENERGY_FOR_REPRODUCTION) {
            double reproductionProbability = (double)rand() / RAND_MAX;
            if (reproductionProbability <= HERBIVORE_REPRODUCTION_PROBABILITY) {
                // Encontre uma célula vazia adjacente aleatoriamente para colocar a prole
                int newRow, newCol;
                bool foundEmptyCell = false;
                int attempts = 0;
                while (!foundEmptyCell && attempts < 4) {
                    int direction = rand() % 4; // 0: cima, 1: baixo, 2: esquerda, 3: direita
                    newRow = row;
                    newCol = col;
                    if (direction == 0) {
                        newRow--;
                    } else if (direction == 1) {
                        newRow++;
                    } else if (direction == 2) {
                        newCol--;
                    } else {
                        newCol++;
                    }
                    
                    // Verifique se a célula é válida e está vazia
                    if (newRow >= 0 && newRow < NUM_ROWS && newCol >= 0 && newCol < NUM_ROWS && entity_grid[newRow][newCol].type == empty) {
                        foundEmptyCell = true;
                    }
                    
                    attempts++;
                }
                
                if (foundEmptyCell) {
                    // Crie a prole na célula vazia adjacente
                    entity_grid[newRow][newCol] = {herbivore, MAXIMUM_ENERGY, 0};
                    // Desconte o custo de energia da reprodução
                    currentEntity.energy -= 10;
                }
            }
        }   
                // Atualize a idade do herbívoro
                currentEntity.age++;
            }
                } else if (currentEntity.type == carnivore) {
                    // Regras para carnívoros
    // <YOUR CODE HERE>
    
    if (currentEntity.age >= CARNIVORE_MAXIMUM_AGE || currentEntity.energy <= 0) {
        // Carnívoro morre após 80 etapas de tempo ou se a energia chegar a 0
        currentEntity = {empty, 0, 0};
    } else {
        // Simulate carnivore move based on the carnivore_move_probability
        if (random_action(carnivore_move_probability)) {
            // Encontre uma célula adjacente aleatória (incluindo células com herbívoros)
            int newRow, newCol;
            bool foundEmptyCell = false;
            int attempts = 0;
            while (!foundEmptyCell && attempts < 4) {
                int direction = rand() % 4; // 0: cima, 1: baixo, 2: esquerda, 3: direita
                newRow = row;
                newCol = col;
                if (direction == 0) {
                    newRow--;
                } else if (direction == 1) {
                    newRow++;
                } else if (direction == 2) {
                    newCol--;
                } else {
                    newCol++;
                }
                
                // Verifique se a célula é válida
                if (newRow >= 0 && newRow < NUM_ROWS && newCol >= 0 && newCol < NUM_ROWS) {
                    foundEmptyCell = true;
                }
                
                attempts++;
            }
            
            if (foundEmptyCell) {
                // Movimente o carnívoro para a célula adjacente
                entity_grid[newRow][newCol] = currentEntity;
                entity_grid[row][col] = {empty, 0, 0};
                currentEntity = {empty, 0, 0}; // A energia é gasta no movimento
            }
        } else {
            // O carnívoro não se move nesta iteração, mas gasta energia
            currentEntity.energy -= 5;
        }
        
        // Verifique a probabilidade de alimentação
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int adjRow = row + i;
                int adjCol = col + j;
                if (adjRow >= 0 && adjRow < NUM_ROWS && adjCol >= 0 && adjCol < NUM_ROWS &&
                    entity_grid[adjRow][adjCol].type == herbivore) {
                    // Carnívoro come o herbívoro
                    currentEntity.energy += 20;
                    // Remova o herbívoro
                    entity_grid[adjRow][adjCol] = {empty, 0, 0};
                }
            }
        }
        
        // Verifique a probabilidade de reprodução
        if (currentEntity.energy > 20) {
            double reproductionProbability = (double)rand() / RAND_MAX;
            if (reproductionProbability <= CARNIVORE_REPRODUCTION_PROBABILITY) {
                // Encontre uma célula vazia adjacente aleatoriamente para colocar a prole
                int newRow, newCol;
                bool foundEmptyCell = false;
                int attempts = 0;
                while (!foundEmptyCell && attempts < 4) {
                    int direction = rand() % 4; // 0: cima, 1: baixo, 2: esquerda, 3: direita
                    newRow = row;
                    newCol = col;
                    if (direction == 0) {
                        newRow--;
                    } else if (direction == 1) {
                        newRow++;
                    } else if (direction == 2) {
                        newCol--;
                    } else {
                        newCol++;
                    }
                    
                    // Verifique se a célula é válida e está vazia
                    if (newRow >= 0 && newRow < NUM_ROWS && newCol >= 0 && newCol < NUM_ROWS && entity_grid[newRow][newCol].type == empty) {
                        foundEmptyCell = true;
                    }
                    
                    attempts++;
                }
                
                if (foundEmptyCell) {
                    // Crie a prole na célula vazia adjacente
                    entity_grid[newRow][newCol] = {carnivore, MAXIMUM_ENERGY, 0};
                    // Desconte o custo de energia da reprodução
                    currentEntity.energy -= 10;
                }
            }
        }
                
                // Atualize a idade do carnívoro
                currentEntity.age++;
            }
                }
            }
        }

        // Return the JSON representation of the entity grid
        nlohmann::json json_grid = entity_grid;
        return json_grid.dump(); });
    app.port(8080).run();

    return 0;
}
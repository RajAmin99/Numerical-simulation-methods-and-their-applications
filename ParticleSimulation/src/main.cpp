#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <random>
#include <fstream>

// To simulate position of particles
struct Position {
    double x, y;
};

const std::string csvTracking_FilePath = "C:\\Users\\rajam\\OneDrive - University of Reading\\THIRD YEAR\\PPR\\Numerical-simulation-methods-and-their-applications\\ParticleSimulation\\data\\energy_data.csv";
const std::string rdfTracking_FilePath = "C:\\Users\\rajam\\OneDrive - University of Reading\\THIRD YEAR\\PPR\\Numerical-simulation-methods-and-their-applications\\ParticleSimulation\\data\\rdf_histogram.csv";

// Random Number Generation (Mersenne Twister)
// Using a seed of "32005096" for reproducability 

// std::random_device rd; // Random seed used during programming and testing
std::mt19937 gen(32005096);
std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);

int initialisation(int NumParticles, double Density, std::vector<Position>& particles, double& box_length) {
    
    // Calculation of the simulation box length
    // Density (rho) = N / Area. Therefore, Area = N / Density.
    // The length of one side of a square is the square root of its area.
    box_length = std::sqrt(NumParticles / Density);

    // Determining lattice size and spacing
    int n_cells = std::ceil(std::sqrt(NumParticles)); 
    double spacing = box_length / n_cells;

    // Looping through the grid and place particles
    int p = 0; // Particle counter
    
    for (int x = 0; x < n_cells; ++x) {
        for (int y = 0; y < n_cells; ++y) {
                
            // Only place a particle if NumParticles not yet reached
            if (p < NumParticles) {
                Position new_pos;
                new_pos.x = (x + 0.5) * spacing;
                new_pos.y = (y + 0.5) * spacing;
                
                particles.push_back(new_pos);
                p++;
            }
        }
    }
    
    std::cout << "Initialization complete. Placed " << particles.size() << " particles.\n";
    return 0;
}

double calc_energy(int idx, const std::vector<Position>& particles, double box_length) {
    // Calculation of a Particle's Energy
    double energy = 0.0;
    double rc2 = 6.25; // Cutoff radius squared (2.5 * 2.5)

    for (int i = 0; i < particles.size(); ++i) {
        if (i == idx) continue; // prevent self-interaction of a particle

        // Calculate 2D distance components
        double dx = particles[idx].x - particles[i].x;
        double dy = particles[idx].y - particles[i].y;

        // Periodic Boundary Conditions (Minimum Image Convention)
        dx -= box_length * std::round(dx / box_length);
        dy -= box_length * std::round(dy / box_length);

        double r2 = dx*dx + dy*dy;

        // Lennard-Jones Potential Calculation (only if within cutoff)
        if (r2 < rc2) {
            double r6_inv = 1.0 / (r2 * r2 * r2);
            energy += 4.0 * (r6_inv * r6_inv - r6_inv); 
        }
    }
    return energy;
}

// Test initial energy
double calc_total_energy(const std::vector<Position>& particles, double box_length) {
    double total_energy = 0.0;
    for (int i = 0; i < particles.size(); ++i) {
        total_energy += calc_energy(i, particles, box_length);
    }
    // Divide by 2 to account for pairs
    return total_energy / 2.0; 
}

void mcmove(std::vector<Position>& particles, double box_length, double Temp, double max_disp, int& accepted_moves) {
    
    // Picking a random particle uniformly
    std::uniform_int_distribution<int> particle_dist(0, particles.size() - 1);
    int idx = particle_dist(gen);

    // Calculation of particle energy in current state
    double e_old = calc_energy(idx, particles, box_length);

    // Saving old position in case of rejection
    Position old_pos = particles[idx];

    // Random displacement proposition
    particles[idx].x += (uniform_dist(gen) - 0.5) * max_disp;
    particles[idx].y += (uniform_dist(gen) - 0.5) * max_disp;

    // PBC
    particles[idx].x -= box_length * std::floor(particles[idx].x / box_length);
    particles[idx].y -= box_length * std::floor(particles[idx].y / box_length);

    // Energy calculation in proposed new state
    double e_new = calc_energy(idx, particles, box_length);
    double delta_e = e_new - e_old;

    // Metropolis Acceptance Criteria
    // If delta_e negative, condition is true, accepted
    // If delta_e positive, Boltzmann probability checked
    if (delta_e < 0.0 || uniform_dist(gen) < std::exp(-delta_e / Temp)) {
        accepted_moves++; // Move accepted, particle stays
    } else {
        particles[idx] = old_pos; // Move rejected, og position restored
    }
}

void simulate_withTracking(int Cycles, int NumParticles, std::vector<Position>& particles, double box_length, double Temp, double max_disp, int& accepted_moves) {
    
    std::ofstream outfile(csvTracking_FilePath);
    outfile << "Cycle,TotalEnergy\n";

    // Setup for RDF Histogram variables
    int num_bins = 100;
    double max_r = box_length / 2.0; // Max radius to measure is half the box
    double bin_width = max_r / num_bins;
    std::vector<double> rdf_histogram(num_bins, 0.0);
    int rdf_samples = 0;

    std::cout << "Running " << Cycles << " MC cycles and writing data files...\n";
    
    for (int c = 0; c < Cycles; ++c) {
        for (int i = 0; i < NumParticles; ++i) {
            mcmove(particles, box_length, Temp, max_disp, accepted_moves);
        }
        
        // Data Sampling (Every 10 cycles)
        if (c % 10 == 0) {
            double current_energy = calc_total_energy(particles, box_length);
            outfile << c << "," << current_energy << "\n";

            // RDF Sampling
            rdf_samples++;
            for (int i = 0; i < NumParticles - 1; ++i) {
                for (int j = i + 1; j < NumParticles; ++j) {
                    
                    double dx = particles[i].x - particles[j].x;
                    double dy = particles[i].y - particles[j].y;
                    
                    // PBC for distance measurement
                    dx -= box_length * std::round(dx / box_length);
                    dy -= box_length * std::round(dy / box_length);
                    
                    double r = std::sqrt(dx*dx + dy*dy);

                    if (r < max_r) {
                        int bin = static_cast<int>(r / bin_width);
                        rdf_histogram[bin] += 2.0; // +2 doubled pairs
                    }
                }
            }
        }
    }

    outfile.close();

    // Write to csv
    std::ofstream rdf_file(rdfTracking_FilePath);
    rdf_file << "Distance,Count\n";
    for (int b = 0; b < num_bins; ++b) {
        double r_center = (b + 0.5) * bin_width;
        double average_count = rdf_histogram[b] / rdf_samples; 
        rdf_file << r_center << "," << average_count << "\n";
    }
    rdf_file.close();

    std::cout << "Data collection complete. Files closed successfully.\n";
}

int main() {
    // Variables
    double Temp = 1.0;
    int NumParticles = 150;
    double Density = 0.8;    
    int Cycles = 5000;
    double max_disp = 0.25; // Maximum allowed distance a particle can jump in one step

    std::vector<Position> particles;
    double box_length = 0.0;

    // Initialisation
    initialisation(NumParticles, Density, particles, box_length);
    std::cout << "Initial System Energy: " << calc_total_energy(particles, box_length) << "\n\n";

    // Simulation running
    int accepted_moves = 0;
    int total_moves_attempted = NumParticles * Cycles;
    
    simulate_withTracking(Cycles, NumParticles, particles, box_length, Temp, max_disp, accepted_moves);

    // Final Output
    std::cout << "\nFinal System Energy: " << calc_total_energy(particles, box_length) << "\n";
    std::cout << "Acceptance Ratio: " << (double)accepted_moves / total_moves_attempted * 100.0 << "%\n";

    return 0;
}
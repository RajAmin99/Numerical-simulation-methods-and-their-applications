# Libraries ====
library(ggplot2)

# Setting Up Files ====
assets <- "C://Users/rajam/OneDrive - University of Reading/THIRD YEAR/PPR/Numerical-simulation-methods-and-their-applications/assets"

particle.Energy.file <- "ParticleSimulation/data/energy_data.csv"
particle.Energy <- read.csv(particle.Energy.file, header = T)
attach(particle.Energy)

rdf.Data.file <- "ParticleSimulation/data/rdf_histogram.csv"
rdf.Data <- read.csv(rdf.Data.file, header = T)
attach(rdf.Data)

# Total Energy vs. Cycle ====
particle.Energy.plot <- ggplot(
    particle.Energy,
    aes(x = Cycle, y = TotalEnergy)) +
    geom_line(color = "steelblue") +
    geom_smooth(method = "loess",
    color = "red",
    se = F) +
    labs(title = "System Equilibration: Total Energy vs. Cycles",
         x = "Monte Carlo Cycle", 
         y = "Total Energy") +
    theme_minimal()

## Saving
assets.Particle <- paste0(assets, "/particle_Energy_plot.png")
ggsave(assets.Particle, plot = particle.Energy.plot, width = 15, height = 8, units = "cm")


# Radial Distribution Function ====

## Simulation parameters from main code
density <- 0.8
bin_width <- rdf.Data$Distance[2] - rdf.Data$Distance[1]

# Normalize raw counts to g(r)
rdf.Data$IdealCount <- (2 * pi * rdf.Data$Distance * bin_width) * density
rdf.Data$g_r <- rdf.Data$Count / rdf.Data$IdealCount
rdf.Data$g_r[is.nan(rdf.Data$g_r) | is.infinite(rdf.Data$g_r)] <- 0

# Plot Radial Distribution Function ====
rdf.Data.plot <- ggplot(rdf.Data, aes(x = Distance, y = g_r)) +
    geom_line(color = "forestgreen", size = 1) +
    geom_hline(yintercept = 1, linetype = "dashed", color = "black") + # Ideal gas limit
    labs(title = "Radial Distribution Function g(r)",
         x = "Distance (r)", 
         y = "g(r)") +
    theme_minimal()

# Save the plot
assets.rdf <- paste0(assets, "/rdf_Data_plot.png")
ggsave(assets.rdf, plot = rdf.Data.plot, width = 15, height = 8, units = "cm")
# Libraries
library(ggplot2)

# Setting Up Files

particle.Energy.file <- "ParticleSimulation/data/energy_data.csv"
particle.Energy <- read.csv(particle.Energy.file, header = T)
attach(particle.Energy)

plot()

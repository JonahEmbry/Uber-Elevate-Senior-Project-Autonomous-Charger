# UBER Elevate Senior Capstone Project: Autonomous Charging Vehicle


This project was done in collaboration with Uber Elevate and a group of Duke University students. Our goal is to design, fabricate, and test a system capable of navigating to a location (Long Range System), elevating to a given height (Z-stage), and plug a charger into a charging port (Close Range System). The overall system architecture was broken down in three subgroups: Long Range System (LRS), Z-Stage (ZS), and Close Range System (CRS). Each of these subsystems features it's own microprocessor. We decided to use the Arduino platform due to it's affordability and the wide variety of supporting material. An I2C protocol to communicate between the different subgroups.


# Long Range System


# Z-Stage


# Close Range System


# Overall Control Flow

The LRS microcontroller serves as the central communications hub, or Master controller. The ZS and CRS controllers are therefore Slaves. The CONOPS is as follows:

User Input Start -> LRS Movement -> LRS Communication to ZS -> ZS Elevation -> ZS Communication to LRS -> LRS Communication to CRS -> CRS Movement

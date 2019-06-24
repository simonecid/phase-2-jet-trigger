import numpy as np

# generates grid of random numbers
#event = np.random.randint(0, 30, (72, 96))
event = np.random.normal(loc=30, scale=10, size=(72, 96))
#event = np.full((72, 96), 1)

event[36][0] = 100
event[0][48] = 100
event[36][48] = 100
event[36][95] = 100
event[71][48] = 100

# save event to file
np.savetxt("event.txt", event, fmt="%u")
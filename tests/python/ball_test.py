import time

import sys
sys.path.append('../../Release')

from Lobot import Ball, vec3

b = Ball();

b.position         = vec3(0, 0, 1000);
b.velocity         = vec3(0, 0, 0);
b.angular_velocity = vec3(0, 0, 0);

# time step, in seconds
dt = 0.0166

n = 300;

for i in range(1, n):

    # advance through time
    b.step(dt);
    
    print(b.position[2]);

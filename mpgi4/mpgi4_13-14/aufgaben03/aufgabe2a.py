 # -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation


fig = plt.figure(figsize=(15,5))
ax = plt.subplot2grid((1,4),(0, 0))
ax.set_xlim(-0.5,0.5)
ax.set_ylim(-2,2)
spine, = ax.plot([], [], lw=2)
left_arm, = ax.plot([], [], lw=2)
right_arm, = ax.plot([], [], lw=2)
left_leg, = ax.plot([], [], lw=2)
right_leg, = ax.plot([], [], lw=2)

px = plt.subplot2grid((1,4),(0, 1), colspan=3)
px.set_xlim(0,200)
px.set_ylim(-0.4,0.4)
curves = [px.plot([],[],"-")[0] for i in range(11)]

# 21 schlüsselbilder
keytime = np.linspace(0, 200, 21)
keyframe = []
keyframe.append(np.array([0.,-0.05, -0.2,-0.2, 0.2,-0.2, 0.25, -0.3, 0.3, 0.1, 0.2]))
keyframe.append(np.array([0.,0.0, 0.2,-0.1, -0.2,-0.1, 0.1, 0.1,0.2, -0.3,0.3]))
for i in range(9):
    keyframe.append(keyframe[0])
    keyframe.append(keyframe[1])
keyframe.append(keyframe[0])
assert(len(keytime) == len(keyframe))


def rotate( v, angle ):
    s = np.sin(angle)
    c = np.cos(angle)
    rv = np.array([v[0] * c - v[1] * s, v[0] * s + v[1] * c])
    return rv


def param2pos( param ):
    """
    Computes positions of joints for the stick guy.
    Inputs:
    param : list of parameters describing the pose
    param[0]: height of hip
    param[1]: angle of spine to vertical axis
    param[2]: angle of upper arm 0 to spine
    param[3]: angle of lower arm 0 to upper arm 0
    param[4,5]: as above, other arm
    param[6]: angle of neck/head to spine
    param[7]: angle of upper leg 0 to vertical axis
    param[8]: angle of lower leg 0 to upper leg 0
    param[9,10]: as above, other leg
    """

    hip_pos = np.array([0.0, param[0]])
    spine_vec = np.array([0.0,  1.0])
    spine_vec = rotate(spine_vec, param[1])
    neck_pos = hip_pos + spine_vec
    basic_arm_vec = -0.6*spine_vec
    arm_vec = rotate(basic_arm_vec, param[2])
    left_elbow_pos = neck_pos + arm_vec
    arm_vec = rotate(arm_vec, param[3])
    left_hand_pos = left_elbow_pos + arm_vec
    lad = np.array([neck_pos,left_elbow_pos,left_hand_pos])
    left_arm.set_data(lad[:,0],lad[:,1])

    arm_vec = rotate(basic_arm_vec, param[4])
    right_elbow_pos = neck_pos + arm_vec
    arm_vec = rotate(arm_vec, param[5])
    right_hand_pos = right_elbow_pos + arm_vec
    rad = np.array([neck_pos,right_elbow_pos,right_hand_pos])
    right_arm.set_data(rad[:,0],rad[:,1])

    neck_vec = 0.3*spine_vec
    neck_vec = rotate( neck_vec, param[6])
    head_pos = neck_pos + neck_vec
    sd = np.array([hip_pos,neck_pos,head_pos])
    spine.set_data(sd[:,0],sd[:,1])

    basic_leg_vec = (0.0,-0.7)
    leg_vec = rotate(basic_leg_vec, param[7])
    left_knee_pos = hip_pos + leg_vec
    leg_vec = rotate(leg_vec, param[8])
    left_foot_pos = left_knee_pos + leg_vec
    lld = np.array([hip_pos,left_knee_pos,left_foot_pos])
    left_leg.set_data(lld[:,0],lld[:,1])
    leg_vec = rotate(basic_leg_vec, param[9])
    right_knee_pos = hip_pos + leg_vec
    leg_vec = rotate(leg_vec, param[10])
    right_foot_pos = right_knee_pos + leg_vec
    rld = np.array([hip_pos,right_knee_pos,right_foot_pos])
    right_leg.set_data(rld[:,0],rld[:,1])


def animate(t):
    curframe = interpolate(t)
    param2pos(curframe)
    global curves_x, curves_y
    if t == 0:
        curves_x = [float(t)]
        curves_y = curframe
    else:
        curves_x.append(float(t))
        curves_y = np.c_[curves_y, curframe]
    for i in range(len(curves)):
        curves[i].set_data(curves_x, curves_y[i])

    return left_arm,right_arm,spine,left_leg, right_leg


def interpolate(t):
    #Hier ist die lineare interpolation!!!
    k = np.searchsorted(keytime, t, side='right') - 1
    u = (t - keytime[k]) / (keytime[k + 1] - keytime[k])
    curframe = (1.0-u)*keyframe[k] + u*keyframe[k+1]
    return curframe


anim = animation.FuncAnimation(fig, animate, frames=200, interval=10, blit=False)
plt.subplots_adjust(left=0.04, right=0.98, top=0.98, bottom=0.05)
plt.show()

plt.subplot(2,1,1)
tx = np.linspace(0,200, 1000, endpoint=False)
ty = np.zeros((1000,11))
for i, t in enumerate(tx):
    ty[i] = interpolate(t)
for i in range(11):
    plt.plot(tx, ty[:,i], '-')
plt.xlim(0,200)
plt.ylim(-0.4,0.4)

plt.subplot(2,1,2)
for i in range(11):
    plt.plot(np.hstack((tx, tx+200.0)), np.hstack((ty[:,i],ty[:,i])), '-')
plt.xlim(180,220)
plt.ylim(-0.4,0.4)

plt.subplots_adjust(left=0.06, right=0.97, top=0.98, bottom=0.05)
plt.savefig('./figures/aufgabe2a.pdf')
plt.show()

#This program is for the 5th degree polynomial
import matplotlib.pyplot as plt
import numpy as np
import serial
import time

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600)
    ser.flush()
    
    qi = input("Initial position= ")
    qi = float(qi)
    qf = input("final position= ")
    qf = float(qf)
    tf = input("final time= ")
    tf = float(tf)

    a0 = qi
    a1 = 0
    a2 = 0
    a3 = 10*(qf-qi)/(tf**3)
    a4 = -15*(qf-qi)/(tf**4)
    a5 = 6*(qf-qi)/(tf**5)

    Ts = 0.01 # sample time
    Ns = tf/Ts
    Ns = int(Ns) #number of samples

    q = np.zeros(Ns)
    dq = np.zeros(Ns)
    ddq = np.zeros(Ns)
    time_sec = np.zeros(Ns)
    t = 0

    for k in range(0,Ns):
        q[k] = a0 + a1*t + a2*t**2 + a3*t**3 + a4*t**4 + a5*t**5
        dq[k] = 3*a3*t**2 + 4*a4*t**3 + 5*a5*t**4
        ddq[k] = 6*a3*t + 12*a4*t**2 + 20*a5*t**3
        time_sec[k] = k*Ts
        t = t + Ts
    print("The q Array is: ", q)
    print("The dq Array is: ", dq)
    print("The ddq Array is: ", ddq)     
    plt.plot(time_sec,q,'r')
    plt.plot(time_sec,dq,'b')
    plt.plot(time_sec,ddq,'k')
    plt.ylabel('q(t)')
    plt.xlabel('t')

    ser.write(str("Ns").encode('utf-8'))
    ser.write(str("\n").encode('utf-8'))
    time.sleep(0.01)
    ser.write(str(Ns).encode('utf-8'))
    ser.write(str("\n").encode('utf-8'))
    time.sleep(0.01)
    
    line = ser.readline().decode('utf-8').rstrip()
    print(line)
    if line == "ok":
        for k in range(0,Ns):
            ser.write(str(dq[k]).encode('utf-8'))
            ser.write(str("\n").encode('utf-8'))
            print(dq[k])
            while True:
                line = ser.readline().decode('utf-8').rstrip()
                print(line)
                if line == "ok":
                    break
    ser.write(str("End").encode('utf-8'))
    ser.write(str("\n").encode('utf-8'))
    
    while True:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        
    ser.close()
    plt.show()
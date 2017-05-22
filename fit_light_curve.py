import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy import interpolate
import matplotlib
from scipy import integrate


#set datafile name here
datafile = 'onestar.txt'


font = {'family' : 'normal',
        'weight' : 'normal',
        'size'   : 22}

matplotlib.rc('font', **font)


#SN Luminosity function
def lum_early(t,t0,Mni,Mej):

    if t0<-10:
        return 0*t

    t = t-t0

    vej = 1.5e9 #cm/s
    Mej = Mej * 1.988e33

    tau = np.sqrt(2. * 0.02 * Mej / (13.6 * 3.e10 * vej)) / 86400.

    fake_t = np.linspace(0,max(t),100)


    Mni = Mni * 1.988e33#to grams
    tauN = 8.77 #days
    tauC = 111. #days
    x = fake_t/tau
    y = tau/(2.*tauN)
    s = tau * (tauC - tauN) / (2.*tauC * tauN)
    A = 2. * x * np.exp(-2. * x * y + x**2)
    B = 2. * x * np.exp(-2. * x * y + 2. * x * s + x**2)
    fake_lum = Mni * np.exp(-x**2) * ((3.22e10*integrate.cumtrapz(A,x=x,initial=0)) \
					+(6.78e9*integrate.cumtrapz(B,x=x,initial=0)))


    #resample template model
    lum_function = interpolate.interp1d(fake_t,fake_lum,bounds_error = False,fill_value=0.0)

    luminosity = lum_function(t)


    return luminosity

#Load the datafile here
flux = np.loadtxt(datafile,usecols=(2,),unpack=True,delimiter=',')

gind = np.where(flux > 0)
flux = flux[gind]

flux = flux - flux[0]

time = np.arange(0,len(flux)) / 1.0

gind = np.where(time<70)
time = time[gind]
flux = flux[gind]

#Rescale flux to be more like a SN
flux = flux * 1.0e43 / np.nanmax(flux)

#fit the model to the data
popt, pcov = curve_fit(lum_early, time, flux,p0=[0,0.1,1],maxfev=5000)

print 't0',popt[0]
print 'Mni',popt[1]
print 'Mej',popt[2]

plt.plot(time,flux,linewidth=3,label='Data')
plt.plot(time,lum_early(time,popt[0],popt[1],3.0),linewidth=3,label='Model')
plt.yscale('log')
plt.legend(frameon=False,loc=1)
plt.xlabel('Time')
plt.ylabel('Output')
plt.show()
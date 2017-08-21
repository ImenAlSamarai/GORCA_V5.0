
'''
J. A. Aguilar: matplotlib wrapper for ROOT objects: V2.0 (24/08/11)
'''

import ROOT

from pylab import *
import sys
from pylab import *
import matplotlib.path as path
import matplotlib.patches as patches
import numpy as np



def Hist2DtoHist2D(h, ax, **kwargs):
    
    try:
        if not h.InheritsFrom("TH2"):
            raise TypeError("%s does not inherit from TH2" % h)
    except:
        raise TypeError("%s is not a ROOT object" % h)

    
    nx = h.GetNbinsX()
    ny = h.GetNbinsY()

    i, j = ROOT.Long(0), ROOT.Long(0)

    content = array([array([h.GetBinContent(i, j)
                            for i in range(1, nx + 1)])
                     for j in range(1, ny + 1)])
    
    xedges = array([h.GetXaxis().GetBinLowEdge(i)
                    for i in range(1, nx + 2)])
    
    
    yedges = array([h.GetYaxis().GetBinLowEdge(i)
                    for i in range(1, ny + 2)])
    
    xx      = array([(xedges[i+1] + xedges[i])/2
                     for i in range(nx)])
    

    yy      = array([(yedges[i+1] + yedges[i])/2
                     for i in range(ny)])

    return ax.pcolormesh(xx, yy, content, antialiased=True, **kwargs)        
    #return ax.pcolormesh(xx, yy, content, antialiased=True, shading='gouraud', **kwargs)        





def HistToHist(h, ax, bot=1e-8, **kwargs):

    try:
        if not h.InheritsFrom("TH1"):
            raise TypeError("%s does not inherit from TH1" % h)
    except:
        raise TypeError("%s is not a ROOT object" % h)

    nbins = h.GetNbinsX()
    maxbin = h.GetMaximumBin()
    width = h.GetBinWidth(1) 

    x = array([h.GetBinLowEdge(i) for i in range(0, nbins + 2)])
    ww = array([h.GetBinContent(i) for i in range(1, nbins + 1)])

    for i, w in enumerate(ww):
        if w == 0:
            ww[i]  = bot

            
    #Add a last empty bin  
    ww = np.append(ww, bot)
    #ww = np.append(ww, bot)
    ww = np.append(bot, ww)

    return ax.hist(x, x, weights = ww, **kwargs)
    

def HistToErrorBar(h, ax, bot = 1e-8, **kwargs):
    try:
        if not h.InheritsFrom("TH1"):
            raise TypeError("%s does not inherit from TH1" % h)
    except:
        raise TypeError("%s is not a ROOT object" % h)
    
    nbins = h.GetNbinsX()

    x = array([h.GetBinCenter(i) for i in range(1, nbins + 1)])

    weights = array([h.GetBinContent(i) for i in range(1, nbins + 1)])

    ylow = array([h.GetBinError(i) for i in range(1, nbins + 1)])
    xerr = array([0 for i in range(1, nbins + 1)])

    yhigh = ylow

    for i, err in enumerate(ylow):
        if weights[i] - ylow[i] < bot:
            ylow[i] = weights[i] - bot
    
    return ax.errorbar(x, weights, xerr= [xerr, xerr], yerr = [ylow, yhigh], **kwargs) 


def HistToLine(h, ax, **kwargs):

    try:
        if not h.InheritsFrom("TH1"):
            raise TypeError("%s does not inherit from TH1" % h)
    except:
        raise TypeError("%s is not a ROOT object" % h)

    nbins = h.GetNbinsX()
    
    xx = [h.GetBinCenter(i) for i in range(1, nbins + 1)]
    yy = [h.GetBinContent(i) for i in range(1, nbins + 1)]

    return ax.plot(xx, yy, **kwargs)


def GraphToLine(g, ax, **kwargs):

    try:
        if not g.InheritsFrom("TGraph"):
            raise TypeError("%s does not inherit from TGraph" % g)
    except:
        raise TypeError("%s is not a ROOT object" % g)
    
    
    xx = []
    yy = []

    n = arange(0, g.GetN(), 1)
    
    for i in n:
        x, y = ROOT.Double(0), ROOT.Double(0)
        g.GetPoint(int(i), x, y)
        xx.append(x)
        yy.append(y)

    return ax.plot(xx, yy, **kwargs)


def ErrorGraphToErrorBar(g, ax, **kwargs):

    try:
        if not g.InheritsFrom("TGraph"):
            raise TypeError("%s does not inherit from TGraph" % g)
    except:
        raise TypeError("%s is not a ROOT object" % g)
    
    
    xx = []
    yy = []

    yerror = []
    xerror = []
    n = arange(0, g.GetN(), 1)

    for i in n:
        x, y = ROOT.Double(0), ROOT.Double(0)
        g.GetPoint(int(i), x, y)
        xx.append(x)
        yy.append(y)

        yerror.append(g.GetErrorY(int(i))/2.)
        xerror.append(g.GetErrorX(int(i))/2.)
        
    return ax.errorbar(xx, yy, xerr = [xerror,xerror], yerr=[yerror, yerror], **kwargs)




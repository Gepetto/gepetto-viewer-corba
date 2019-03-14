from .gepetto.corbaserver import GraphicalInterface
from .gepetto__POA.corbaserver import NodeCallback as POA_NodeCallback
from .gepetto import Error

from .client import Client

## Helper class
class Color(object):
    # Define some RGBA-normalized color (osg convention)
    white       = [1.0,1.0,1.0,1.0]
    lightWhite  = [0.9,0.9,0.9,1.0]
    green       = [0.0,1.0,0.0,1.0]
    lightGreen  = [0.0,0.9,0.0,1.0]
    yellow      = [1.0,1.0,0.0,1.0]
    lightYellow = [0.9,0.9,0.0,1.0]
    blue        = [0.0,0.0,1.0,1.0]
    lightBlue   = [0.0,0.0,0.9,1.0]
    grey        = [0.7,0.7,0.7,1.0]
    lightGrey   = [0.7,0.7,0.7,0.7]
    red         = [1.0,0.0,0.0,1.0]
    lightRed    = [0.9,0.0,0.0,1.0]
    black       = [0.0,0.0,0.0,1.0]
    lightBlack  = [0.1,0.1,0.1,1.0]
    brown       = [0.85,0.75,0.15,1.0]
    lightBrown  = [0.75,0.65,0.10,1.0]
    
    def __init__(self):
        self.colors = (
                self.white       ,
                self.lightWhite  ,
                self.green       ,
                self.lightGreen  ,
                self.yellow      ,
                self.lightYellow ,
                self.blue        ,
                self.lightBlue   ,
                self.grey        ,
                self.lightGrey   ,
                self.red         ,
                self.lightRed    ,
                self.black       ,
                self.lightBlack  ,
                self.brown       ,
                self.lightBrown  ,
                )

    def __getitem__ (self, i):
        return self.colors[i]

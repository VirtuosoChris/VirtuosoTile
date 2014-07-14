import json
import PIL

def blit(outputatlas,tilein,xcoord,ycoord,tilesize):
	pass

class Tilesource(object):
	
	def __init__(self,fn):
		#if an ts file exists, load it.
		#otherwise treat it like a regex and read all files that 
		#count 
		tsfile=os.path.join(fn,'.ts')
		self.imagefile=fn
		self.imagedata=image.load(fn)
		
		if(os.path.exists(tsfile)):
			self.loadtsfile(tsfile)
		else:
			self.width=1
			self.height=1
			self.tiledim=self.imagedata.dims()
		
		self.tiles=[]
	def gettile(self,index):
		xcoord=index % self.width
		ycoord=index % self.height
		return self.imagedata.subimage(xcoord*self.tiledim,ycoord*self.tiledim,self.tiledim,self.tiledim)

	

		
class LevelFile(object):
	def __init__(self,fn):
		self.dictionary=json.loads(open(fn,'r'))
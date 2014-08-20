import json
from PIL import Image
import os
import re
import random

	
def chop_image(im,tiledim):
	frames=(int(im.size[0] / tiledim),int(im.size[1] / tiledim))
	#warn if cannot be chopped (if there is a remainder
	if(im.size[0] % tiledim or im.size[1] % tiledim):
		print("Warning, image cannot be chopped")
		
	images=[]
	for y in range(frames[1]):
		for x in range(frames[0]):
			images.append(im.crop((x*tiledim,y*tiledim,(x+1)*tiledim,(y+1)*tiledim)))
	return images

def get_tilesources(fn,tiledim):
	tsfile=os.path.join(fn,'.ts')
	current_tilesources={} #the tilesources are a dictionary that maps keys to a list of images.
	
	if(os.path.isdir(fn)):
		subsources={}
		for root, dirs, files in os.walk(fn):
			for f in files:
				tiles=get_tilesources(root+'/'+f,tiledim)
				subsources.update(tiles)
		return subsources
	elif(os.path.splitext(fn)[1]=='ts'):
		#parse tsfile and return dictionary
		tsjson=json.load(open(fn,'r'))
		subsources={}
		#pfx=os.path.dirname(fn)
		for k,v in tsjson:
			file=v["image"]
			indices=v["indices"]
			im=Image.open(file)
			v=chop_image(im)
			subsources[k]=[v[x] for x in indices]
		return subsources
		
	elif(os.path.exists(tsfile)):
		return get_tilesources(tsfile)	#ignore file and parse as tsfile
	else:
		try:
			im=Image.open(fn)
			k=os.path.splitext(fn)[0]
			v=chop_image(im,tiledim)
			return {k:v}
		except:
			return {}

def count_paren_parity(strin):
	n=0
	for s in strin:
		if(s==')'):
			n-=1
		if(s=='('):
			n+=1
	return n

tileset_regex=re.compile('<(\S+?)>')
def alias_string(aliases,stringin):
	curparity=0
	sout='['
	for s in stringin.split():
		if(s in aliases):
			sout+=aliases[s]+','
		else:
			curparity=count_paren_parity(s)
			sout+=s
			if(curparity==0):
				sout+=','
	sout=tileset_regex.sub(r"tileset_lookup('\1',tiles)",sout)
	return sout[:-1]+']'


def animate_potential(tileset,fps):
	return tileset
def sub_potential(tileset,begin,end):
	return tileset[begin:end]
def random_potential(tileset):
	return tileset
def tileset_lookup_potential(strarg,tilesources):
	return tilesources[strarg]


def animate_code(rangecode,fps,startpoint=0):
	n=rangecode[1]-rangecode[0]
	animate_prefix=1<<31+fps<<20+n<<16 #whatever the appropriate schema is
	return animate_prefix+rangecode[0]+startpoint
	
def sub_code(rangecode,begin,end):
	return [rangecode[0]+begin,rangecode[1]+end]
def random_code(rangecode):
	return random.randint(rangecode[0],rangecode[1]-1)
def tileset_lookup_code(strarg,rangecodes):
	return rangecodes[strarg]
	
	
def safe_eval(s,environment):
	if('__' in s):
		raise RuntimeError('Unsafe String Passed')
	return eval(s,{"__builtins__":None},environment)

#rewrite this, the design sucks.  Use dataflow model not class update
class LevelFile(object):
	def __init__(self,fn):
		self.dictionary=json.load(open(fn,'r'))
		sources=self.dictionary['sources']
		self.tilesize=self.dictionary['tilesize']
		self.tilesources=get_tilesources(sources[0],self.tilesize) #TODO: BUG..sources should be parsed
		self.tilerefs={}
		self.name=os.path.splitext(fn)[0]
		
		for tilekey,tiles in self.tilesources.items():
			tv=[(tilekey,x) for x in range(len(tiles))]
			self.tilerefs[tilekey]=tv
		self.aliases=self.dictionary['aliases']
		self.outputs=self.dictionary['outputs']
		
		for o in self.outputs:
			if('invisible' in o):
				o['invisible_aliased']=alias_string(self.aliases,o['invisible'])
			if('visible' in o):
				va=[]
				for v in o['visible']:
					va.append(alias_string(self.aliases,v))
				o['visible_aliased']=va
		
	def get_all_potentials(self):
		potentials=set()
		potential_library={'animate':animate_potential,'sub':sub_potential,'random':random_potential,'tileset_lookup':tileset_lookup_potential,'tiles':self.tilerefs}
		for o in self.outputs:
			a=safe_eval(o['invisible_aliased'],potential_library)
			potentials|=set([item for sublist in a for item in sublist])
			
			visible=o['visible_aliased']
			for v in visible:
				a=safe_eval(v,potential_library)
				potentials|=set([item for sublist in a for item in sublist])
		return sorted(potentials)

	def compute_level_codes(self,potentials):
		def codeprocess(e):
			try:
				return int(e)
			except:
				return int(e[0])
				
		rangecodes={}
		for i,x in enumerate(potentials):
			rangecodes.setdefault(x[0],[i,i+1])
			rangecodes[x[0]][1]=i+1
		code_library={'animate':animate_code,'sub':sub_code,'random':random_code,'tileset_lookup':tileset_lookup_code,'tiles':rangecodes}
		for o in self.outputs:
			a=safe_eval(o['invisible_aliased'],code_library)
			o['invisible_codes']=[codeprocess(x) for x in a]
			
			
			visible=o['visible_aliased']
			vc=[]
			for v in visible:
				a=safe_eval(v,code_library)
				vc.append([codeprocess(x) for x in a])
			o['visible_codes']=vc
			
	def atlas_size(n):
		for i in range(32):
			if((1 << (2*i)) > n):
				return (1 << i)
				
	def build_atlas(self,potentials):
		n=len(potentials)
		d=atlas_size(n)
		atlasim=Image.new("RGBA",(d*self.tilesize,d*self.tilesize))
		for i,p in enumerate(potentials):
			tile=self.tilesources[p[0]][p[1]]
			xcoord=(i % d)*self.tilesize
			ycoord=(i / d)*self.tilesize
			atlasim.paste(tile,box=(xcoord,ycoord))
			
		return atlasim
		
	def write_map(self,filename,mapints):
		pass
	def write_map_files(self):
		#for each output, convert the image to a big-endian RGBA triple. then output item
		#including invisible outputs to image pixels
		
		#for o in self.outputs:
		pass
		
		
lf=LevelFile('atlasexample.json')

p=lf.get_all_potentials()
lf.compute_level_codes(p)
print()
import json
from PIL import Image,ImageColor
import os
import re
import sys
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


def animate_potential(tileset,fps,startpoint=0):
	return tileset
def sub_potential(tileset,begin,end):
	return tileset[begin:end]
def random_potential(tileset):
	return tileset
def tileset_lookup_potential(strarg,tilesources):
	return tilesources[strarg]


def animate_code(rangecode,fps,startpoint=0):
	n=rangecode[1]-rangecode[0]
	animate_prefix=(1<<31)+(fps<<20)+(n<<16) #whatever the appropriate schema is
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
		self.tilesize=int(self.dictionary['tilesize'])
		self.tilesources=get_tilesources(sources[0],self.tilesize) #TODO: BUG..sources should be parsed
		self.tilerefs={}
		self.name=os.path.splitext(fn)[0]
		
		for tilekey,tiles in self.tilesources.items():
			tv=[(tilekey,x) for x in range(len(tiles))]
			self.tilerefs[tilekey]=tv
			
		#TODO: verify aliases
		self.aliases=self.dictionary['aliases']
		#TODO: verify palette_aliases
		self.palette_aliases=self.dictionary['palette_aliases']		#http://pillow.readthedocs.org/en/latest/reference/ImageColor.html 
		self.npalette_aliases=dict([(ImageColor.getrgb(cstring),v) for cstring,v in self.palette_aliases.items()])
		self.outputs=self.dictionary['outputs']
		
		for o in self.outputs:
			if('invisible' in o):
				o['invisible_aliased']=alias_string(self.aliases,o['invisible'])
			if('visible' in o):
				if(isinstance(o['visible'],str)):
					o['visible']=self.source_map_to_aliastext(o['visible'])
				va=[]
				for v in o['visible']:
					va.append(alias_string(self.aliases,v))
				o['visible_aliased']=va
	
	def source_map_to_aliastext(self,fn):
		sourcemap=Image.open(fn)
		
		def distfunc(color,pixel):
			return sum(map(lambda x,y:(x-y)*(x-y),color,pixel))
		outaliastext=[]
		outrow=''
		npak=self.npalette_aliases.keys()
		for i,pv in enumerate(sourcemap.getdata()):
			pdex=min(npak,key=lambda v:distfunc(pv,v))
			outrow+=self.npalette_aliases[pdex]
			outrow+=' '
			if(i % sourcemap.size[0] == (sourcemap.size[0]-1)):
				outaliastext.append(outrow.strip())
				outrow=''
		return outaliastext
		
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
			
				
	def build_atlas(self,potentials):
		def atlas_size(n):
			for i in range(32):
				if((1 << (2*i)) > n):
					return (1 << i)
		n=len(potentials)
		d=atlas_size(n)
		atlasim=Image.new("RGBA",(d*self.tilesize,d*self.tilesize))
	
		for i,p in enumerate(potentials):
		
			tile=self.tilesources[p[0]][p[1]]
			xcoord=int((i % d))*self.tilesize
			ycoord=int((i / d))*self.tilesize
	
			atlasim.paste(tile,box=(xcoord,ycoord))
			
		return atlasim
		
	def build_map(self,mapints):
		height=len(mapints)
		width=len(mapints[0])
		
		bb=bytes([ pcolor for row in mapints for column in row for pcolor in [(column >> 24) & 0xFF,(column >> 16) & 0xFF,(column >> 8) & 0xFF,column & 0xFF]])

		return Image.frombytes('RGBA',(width,height),bb)
		
	def build_preview(self,mapints,potentials):
		height=len(mapints)
		width=len(mapints[0])
		
		previmg=Image.new('RGBA',(width*self.tilesize,height*self.tilesize))
		for y,row in enumerate(mapints):
			for x,base in enumerate(row):
				p=potentials[base & 0xFFFF]
				tile=self.tilesources[p[0]][p[1]]
				coords=(x*self.tilesize,y*self.tilesize)
				previmg.paste(tile,box=coords)
		return previmg
	
	def compileall(self,imgtype='PNG',previews=False):
		potentials=self.get_all_potentials()
		self.compute_level_codes(potentials)
		ext=imgtype.lower()
		self.build_atlas(potentials).save(self.name+'.atlas.'+ext,imgtype)
		for o in self.outputs:
			oname=o['name']
			self.build_map(o['visible_codes']).save(self.name+'.'+oname+'.visible.'+ext,imgtype)
			self.build_map([o['invisible_codes']]).save(self.name+'.'+oname+'.invisible.'+ext,imgtype)
			if(previews):
				self.build_preview(o['visible_codes'],potentials).save(self.name+'.'+oname+'.prev.'+ext,imgtype)
	
		
if __name__=='__main__':
	lf=LevelFile(sys.argv[1])
	lf.compileall(previews=True)
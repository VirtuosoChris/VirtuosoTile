import json
from PIL import Image,ImageColor
import os
import re
import sys
import random
from copy import deepcopy

	
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

class TilePixel(object):
	def __init__(self,lower,upper):
		self.lower=lower
		self.upper=upper
		self.fps=0
		self.repeat=False
	def encode_to_pixel(self,atlas_sz):
		i=self.lower & 0xFFFF
		num_frames=(self.upper-self.lower) & 0xFF
		fps=self.fps & 0x7F
		repeat=int(self.repeat)
		r=int(i % atlas_sz)
		g=int(i / atlas_sz)
		b=num_frames
		a=fps | (repeat << 7)
		return [r,g,b,a]
	
#todo: use something a struct for the codes to carry semantic info till compile time instead of encoding it into the ints
def animate_code(tp,fps,startpoint=0,repeat=False):
	tp=deepcopy(tp)
	tp.fps=fps
	tp.lower+=startpoint
	tp.repeat=repeat
	return tp
	
def sub_code(tp,begin,end):
	tp=deepcopy(tp)
	tp.lower+=begin
	tp.upper=tp.lower+end
	return tp
def random_code(tp):
	tp=deepcopy(tp)
	tp.lower=random.randint(tp.lower,tp.upper-1)
	return tp
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
				ov=o['visible']
				if(isinstance(ov,str) or isinstance(ov,unicode)):
					ov=self.source_map_to_aliastext(ov)

				va=[]
				for v in ov:
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


		rangecodes={}
		for i,x in enumerate(potentials):
			rangecodes.setdefault(x[0],TilePixel(i,i+1))
			rangecodes[x[0]].upper=i+1
		
		code_library={'animate':animate_code,'sub':sub_code,'random':random_code,'tileset_lookup':tileset_lookup_code,'tiles':rangecodes}
		for o in self.outputs:
			a=safe_eval(o['invisible_aliased'],code_library)
			
			o['invisible_codes']=a
			
			
			visible=o['visible_aliased']
			vc=[]
			for v in visible:
				a=safe_eval(v,code_library)
				vc.append(a)
			o['visible_codes']=vc
			
				
	def build_atlas(self,potentials):
		def atlas_size(n):
			for i in range(32):
				if((1 << (2*i)) > n):
					return (1 << i)
		n=len(potentials)
		d=atlas_size(n)
		self.atlas_size=int(d)
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
		mbout=[]
		for row in mapints:
			for pixel in row:
				mbout.extend(pixel.encode_to_pixel(int(self.atlas_size)))

		bb=''.join(map(chr,mbout))
		print(mbout[0:3])
		return Image.frombytes('RGBA',(width,height),bb)
		
	def build_preview(self,mapints,potentials):
		height=len(mapints)
		width=len(mapints[0])
		
		previmg=Image.new('RGBA',(width*self.tilesize,height*self.tilesize))
		for y,row in enumerate(mapints):
			for x,base in enumerate(row):
				p=potentials[base.lower & 0xFFFF]
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

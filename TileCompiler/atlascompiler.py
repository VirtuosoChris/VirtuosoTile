import json
from PIL import Image

def merge_tilesources(tsdict):
	def prepend_dict(prefix,dct)
		return dict([(prefix+'/'+k,v) for k,v in dct.items()])
	z={}
	for k,v in tsdict.items():
		z.update(prepend_dict(k,v))
	return z
	
def chop_image(im,tiledim):
	frames=(im.size[0] / tiledim,im.size[1] / tiledim)
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
				tiles=get_tilesources(os.path.join(root,f),tiledim)
				subsources[root]=tiles
		return merge_tilesources(subsources)
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
			v=chop_image(fn,tiledim)
			return {k:v}
		except:
			return {}
	
def read(s):
    "Read a Scheme expression from a string."
    return read_from(tokenize(s))

parse = read

def tokenize(s):
    "Convert a string into a list of tokens."
    return s.replace('(',' ( ').replace(')',' ) ').split()

def read_from(tokens):
    "Read an expression from a sequence of tokens."
    if len(tokens) == 0:
        raise SyntaxError('unexpected EOF while reading')
    token = tokens.pop(0)
    if '(' == token:
        L = []
        while tokens[0] != ')':
            L.append(read_from(tokens))
        tokens.pop(0) # pop off ')'
        return L
    elif ')' == token:
        raise SyntaxError('unexpected )')
    else:
        return atom(token)

def atom(token):
    "Numbers become numbers; every other token is a symbol."
    try: return int(token)
    except ValueError:
        try: return float(token)
        except ValueError:
            return Symbol(token)

		
class LevelFile(object):
	def __init__(self,fn):
		self.dictionary=json.loads(open(fn,'r'))
		sources=self.dictionary['sources']
		self.tilesources=get_tilesources(sources[0]) #TODO: BUG..sources should be parsed 
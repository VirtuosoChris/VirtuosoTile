{
'tilesize':32,
'sources':
	[	
	'tiles',
	'mycharacter.png',
	],
'aliases':
	{
	'g':'<# <animate(10,10) random(<tiles/grass>,)',
	'f':'<tiles/floor>',
	'w':'animate(2,10)|random(<tiles/water>)',
	}
'outputs':
	{
	'name':'mainland',
	'invisible':['f'],
	'visible':[
			['g g g g g g w w w w w w g g g g g g g w w w w w w w w g g g g g g g w w w w g g w w g g w'],
			['g g g g g w w w w g g g g g g w w w w g g w w w w g g g g g g g g g gwwwwwggggg'],
			['ggggggggggggggggggggggggggggggggggggggggggggg'],
			['ggggggggggggggggggggggggggggggggggggggggggggg'],
			['ggggggggggggggggggggggggggggggggggggggggggggg']
		]
	}
}
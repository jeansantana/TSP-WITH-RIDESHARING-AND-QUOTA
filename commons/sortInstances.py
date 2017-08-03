class SortIntanceFiles(object):
  def __init__(self, files):
    self.files = files

  def getSortedFiles(self):
    # File format TICXX-XX-XX-X.in
    groups = {10: [], 20: [], 30: [], 40: [], 50: []}

    for f in self.files:
      parts = f.split('-')
      groups[int(parts[1])].append(f)
    
    gFiles = []
    for key in sorted(groups):
      gFiles.append( sorted(groups[key]) )
    
    return gFiles

# How to use
# files = ['TIC01-10-10-3.in', 'TIC03-40-40-3.in', 'TIC04-30-30-3.in', 'TIC04-50-50-3.in', 'TIC07-10-10-3.in', 'TIC08-30-30-3.in', 'TIC08-40-40-3.in', 'TIC10-10-10-3.in', 'TIC10-20-20-3.in', 'TIC11-40-40-4.in', 'TIC11-50-50-4.in', 'TIC16-30-30-4.in', 'TIC17-10-10-4.in', 'TIC19-10-10-4.in', 'TIC20-20-20-4.in', 'TIC37-30-30-4.in', 'TIC38-10-10-4.in', 'TIC40-40-40-4.in', 'TIC40-50-50-4.in', 'TIC42-30-30-3.in', 'TIC44-30-30-3.in', 'TIC44-40-40-3.in', 'TIC45-50-50-3.in', 'TIC46-30-30-3.in', 'TIC47-20-20-3.in', 'TIC47-50-50-3.in', 'TIC48-20-20-3.in', 'TIC49-20-20-3.in', 'TIC50-10-10-3.in', 'TIC52-10-10-4.in', 'TIC52-40-40-4.in', 'TIC53-20-20-4.in', 'TIC53-50-50-4.in', 'TIC54-40-40-4.in', 'TIC55-20-20-4.in', 'TIC56-20-20-4.in', 'TIC58-40-40-4.in', 'TIC58-50-50-4.in', 'TIC59-50-50-4.in', 'TIC60-30-30-4.in']
# sif = SortIntanceFiles(files)
# fs = sif.getSortedFiles()
# print fs
import argparse

parser = argparse.ArgumentParser(description="Enter number of videos")
parser.add_argument('-n','--number-of-videos',required=True)
args = parser.parse_args()
num_vids_str = args.number_of_videos
num_vids = int(num_vids_str)
print(num_vids)
f = open('newlist.txt','w')
name_precision = 6
i = 0
while(i < num_vids):
  i_str = str(i)
  i_str_zfilled = i_str.zfill(name_precision)
  line = 'file \'i'+i_str_zfilled+'.avi\'\n'
  f.write(line)
  i = i + 1
f.close()
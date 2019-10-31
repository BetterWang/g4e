#%% md
import uproot
events = uproot.open("g4e_output_evt.root")['events']
array = events.array('gen_prt_count')
print(array)
for event in events.itervalues('gen_prt_count'):
    print(event)
# hit_z = events.array("hit_z").content
# hit_x = events.array("hit_x").content
# hit_y = events.array("hit_y").content
# hit_e_loss = events.array("hit_e_loss").content
#
# from awkward import JaggedArray
# import awkward
# a = JaggedArray.fromiter(events['hit_vol_name'].array())
#
# hit_name = a.flatten().regular().astype('U')
#
# Implementation of the exhaustive search algorithm 
#

import sys, time, json
import orio.main.tuner.search.search
from orio.main.util.globals import *

#-----------------------------------------------------

class Exhaustive(orio.main.tuner.search.search.Search):
    '''The search engine that uses an exhaustive search approach'''

    def __init__(self, params):
        '''
        Instantiate an exhaustive search engine given a dictionary of options.
        @param params: dictionary of options needed to configure the search algorithm.
        '''

        orio.main.tuner.search.search.Search.__init__(self, params)

        self.algorithmName = 'Exhaustive Search'

        self.start_coord = None
      
        # read all algorithm-specific arguments
        self.__readAlgoArgs()

        # complain if the total number of search runs is defined (i.e. exhaustive search
        # only needs to be run once)
        if self.total_runs > 1:
            err('orio.main.tuner.search.exhaustive: the total number of %s search runs must be one (or can be undefined)' %
                   self.__class__.__name__, doexit=True)
            

    #-----------------------------------------------------
    # Method required by the search interface
    def searchBestCoord(self, startCoord=None):
        '''
        Explore the search space and return the coordinate that yields the best performance
        (i.e. minimum performance cost).
        
        @param startCoord: Starting coordinate (optional)
        @return:  A list of coordinates
        '''

        info('\n----- begin exhaustive search -----')

        # get the total number of coordinates to be tested at the same time
        coord_count = 1
        if self.use_parallel_search:
            coord_count = self.num_procs
        top_perf={}
        
        # record the best coordinate and its best performance cost
        best_coord = None
        top_coords = {}
        best_compute_time = self.MAXFLOAT
        corr_transfer  = self.MAXFLOAT
        
        # start the timer
        start_time = time.time()
        
        if startCoord:
            if len(startCoord) != self.total_dims:
                warn("orio.main.tuner.search.exhaustive: " + 
                     "Invalid starting coordinate %s specified," + 
                     " expected %d elements, but was given %d" 
                     % (startCoord, self.total_dims, len(startCoord)))
                startCoord = None
            else:
                coord = startCoord
                
        if not startCoord:
            # start from the origin coordinate (i.e. [0,0,...])
            coord = [0] * self.total_dims 
            
        coords = [coord]
        while len(coords) < coord_count:
            coord = self.__getNextCoord(coord)
            if coord:
                coords.append(coord)
            else:
                break
        
        recFlag = True
        
        # evaluate every coordinate in the search space
        while True:

            # determine the performance cost of all chosen coordinates
            perf_costs = self.getPerfCosts(coords)
                        
            # compare to the best result
            pcost_items = perf_costs.items()
            pcost_items.sort(lambda x,y: cmp(eval(x[0]),eval(y[0])))
            for coord_str, (compute_times,transfer_times) in pcost_items:
                coord_val = eval(coord_str)
                #info('cost: %s' % (compute_times))
                floatNums = [float(x) for x in compute_times]
                transferFloats = [float(x) for x in transfer_times]

                (mean_or_min,num) = Globals().vtime
                if mean_or_min == 'mean':
                    # Use the mean of all times (except the first one)
                    if len(compute_times) == 1:
                        compute_time = sum(floatNums)
                    else:
                        compute_time = sum(floatNums[1:]) / (len(compute_times)-1) # ignore first timing result
                    transfer_time = sum(transferFloats) / len(transfer_times)
                elif mean_or_min == 'min':
                    # Use the n-th minimum time
                    compute_time = min(floatNums)
                    transfer_time = min(transferFloats)
                else:
# TODO <<<<<<< HEAD
                    err('Internal error: unrecognized Globals().vtime value', self, doexit=True)
                    
            
                info('coordinate: %s, compute cost: %s, all costs: %s, transfer time: %s' % (coord_val, compute_time, compute_times, transfer_time))
            
                self.__processTrialTime(coord_val,compute_time,transfer_time)
            
                if compute_time < best_compute_time and compute_times > 0.0:
# TODO =======
                    mastercode='''
                    mean_perf_cost = sum(floatNums[1:]) / (len(perf_cost)-1)
                mean_transfer = sum(transferFloats) / len(transfer_costs)

                info('coordinate: %s, average cost: %s, all costs: %s, average transfer time: %s' % (coord_val, mean_perf_cost, perf_cost, mean_transfer))

                if Globals().meta is not None:
                    co_dict = {'coordinate': coord_val}
                    avg_cost = {'average_cost': mean_perf_cost}
                    all_costs = {'all_costs': perf_cost}
                    mean_xfer = {'mean_transfer': mean_transfer}
                    Globals().metadata.update(co_dict)
                    Globals().metadata.update(avg_cost)
                    Globals().metadata.update(all_costs)
                    Globals().metadata.update(mean_xfer)

                    try:
                        cmd=''
                        if Globals().out_filename is not None:
                            cmd = Globals().out_filename
                            cmd = cmd.replace("%iter", str(Globals().metadata['LastCounter']))
                        if not cmd.strip(): cmd = '.'
                        with open(cmd + '/meta.json', 'w') as outfile:
                            json.dump(Globals().metadata, outfile)
                    except Exception, e:
                        err('orio.search.Exhaustive: failed to execute meta export: "%s"\n --> %s: %s' % (Globals().meta,e.__class__.__name__, e),doexit = False)
                
                if mean_perf_cost < best_perf_cost and perf_cost > 0.0:
'''
# TODO>>>>>>> origin/HEAD
                    best_coord = coord_val
                    best_compute_time = compute_time
                    corr_transfer  = transfer_time
                    info('>>>> best coordinate found: %s, average cost: %e, average transfer time: %s' % (coord_val, compute_time, transfer_time))
            
            # record time elapsed vs best perf cost found so far in a format that could be read in by matlab/octave
            #progress = 'init' if recFlag else 'continue'
            #recFlag = False
            #IOtime = Globals().stats.record(time.time()-start_time, best_compute_time, best_coord, progress)
            # don't include time on recording data in the tuning time
            #start_time += IOtime

            # check if the time is up
            if self.time_limit > 0 and (time.time()-start_time) > self.time_limit:
                info('exhaustive search: time limit reached')
                break

            # get to all the next coordinates in the search space
            coords = []
            while len(coords) < coord_count:
                if coord:
                    coord = self.__getNextCoord(coord)
                if coord:
                    coords.append(coord)
                else:
                    break

            # check if all coordinates have been visited
            if len(coords) == 0:
                break

        # compute the total search time
        search_time = time.time() - start_time

        info('----- end exhaustive search -----')
        
        # record time elapsed vs best perf cost found so far in a format that could be read in by matlab/octave
        #Globals().stats.record(time.time()-start_time, best_compute_time, best_coord, 'done')
        
        # return the best coordinate(s)
        return search_time,len(coords)
    
    # Private methods       
    #--------------------------------------------------
        
    def __readAlgoArgs(self):
        '''To read all algorithm-specific arguments'''
        
        for vname, rhs in self.search_opts.iteritems():
            if vname == 'start_coord':
                if not isinstance(rhs,list):
                    err('%s argument "%s" must be a list of coordinate indices' % (self.__class__.__name__,'start_coord'))
                elif len(rhs) != self.total_dims:
                    err('%s dimension of start_coord must be %d, but was instead %d' % (self.__class__.__name__, self.total_dims, len(rhs)))
                self.start_coord = rhs
            else:
                err('orio.main.tuner.search.exhaustive: unrecognized %s algorithm-specific argument: "%s"' %
                    (self.__class__.__name__, vname), doexit=True)

    #--------------------------------------------------

    def __getNextCoord(self, coord):
        '''
        Return the next neighboring coordinate to be considered in the search space.
        Return None if all coordinates in the search space have been visited.
        
        @return: the next coordinate
        '''
        next_coord = coord[:]
        for i in range(0, self.total_dims):
            ipoint = next_coord[i]
            iuplimit = self.dim_uplimits[i]
            if ipoint < iuplimit-1:
                next_coord[i] += 1
                break
            else:
                next_coord[i] = 0
                if i == self.total_dims - 1:
                    return None
        return next_coord


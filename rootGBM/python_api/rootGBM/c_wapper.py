# coding: utf-8
from __future__ import absolute_import
import ctypes
import numpy as np

from .libpath import find_lib_path

def _load_dll():
    dll_path = find_lib_path()
    if len(dll_path) == 0:
        raise Exception("cannot find LightGBM library")
    
    dll = ctypes.cdll.LoadLibrary(dll_path[0])
    return dll

rootGBM = _load_dll()

 
def _safe_call(ret):
    if ret!=1:
       raise IOError('process break off' )

class Dataset(object):      
    def __init__(self, data, label=None ,max_bin_num=100,exactMode=0):
        if not isinstance(data,np.ndarray) or not isinstance(label,np.ndarray):
            raise TypeError('now support numpy array only' )
        tmp = np.unique(label)
        if len(tmp) !=2 or 1 not in tmp or -1 not in tmp:
            raise ValueError('label value must be [1,-1], other value is unacceptable' )
            
        self.Dataset_ptr = None
        self.Bin_ptr = None
        self.data = data
        self.label = label       
        self._construct(max_bin_num,exactMode)
    def __del__(self):
        if self.Dataset_ptr is not None:
            _safe_call(rootGBM.DatasetFree(self.Dataset_ptr))
            self.Dataset_ptr = None
        if self.Bin_ptr is not None:
            _safe_call(rootGBM.BinFree(self.Bin_ptr))   
            self.Bin_ptr = None
    def _construct(self,max_bin_num,exactMode):
        if self.data is None:
            self.Dataset_ptr = None
            self.Bin_ptr = None
            return

        if isinstance(self.data, np.ndarray):
            self.Bin_ptr =  ctypes.c_void_p()
            self.Dataset_ptr =  ctypes.c_void_p()
            data = np.array(self.data.reshape(self.data.size), dtype=np.float32,copy=False)
            label = np.array(self.label, dtype=np.float32,copy=False)
            ptr_data  = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
            ptr_label = label.ctypes.data_as(ctypes.POINTER(ctypes.c_float))  
            _safe_call(rootGBM.DatasetCreateFromNumpy( ptr_data, ptr_label,ctypes.c_int(self.data.shape[0]),
            ctypes.c_int(self.data.shape[1]),ctypes.c_int(max_bin_num),ctypes.c_int(exactMode),ctypes.byref(self.Bin_ptr),ctypes.byref(self.Dataset_ptr)))
        else:
             raise TypeError('Cannot load datas' )
        return self
 

class rootGBMClassifier(object):
    """"Booster in LightGBM."""
    def __init__(self, dataset=None,learningRat=1,tree_nums=500,threads_num=1,exactMode=0):
        if not isinstance(dataset,Dataset):
            raise TypeError('input data type is not right')
        self.Booster_ptr = ctypes.c_void_p()
        self.dataset = dataset
        self.exactMode =  exactMode      
        _safe_call(rootGBM.CreateBoost(ctypes.c_int(tree_nums),ctypes.c_float(learningRat),ctypes.c_int(threads_num),ctypes.byref(self.Booster_ptr)))          
        self._training()
        
    def _training(self):
        _safe_call(rootGBM.TrainBoost(self.dataset.Dataset_ptr,self.dataset.Bin_ptr, self.Booster_ptr,ctypes.c_int(self.exactMode)))
  
    def predict(self,testdata):
        if len(testdata.shape) != 2:
            raise ValueError('Input must be 2 dimensional') 
        data = np.array(testdata.reshape(testdata.size), dtype=np.float32, copy=False)    
        
        ptr_data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))  
                 
        preds = np.zeros(testdata.shape[0], dtype=np.float32)
 
        _safe_call(rootGBM.pred_np_array(self.Booster_ptr,self.dataset.Bin_ptr,ptr_data, 
         ctypes.c_int(testdata.shape[0]),ctypes.c_int(testdata.shape[1]),
         preds.ctypes.data_as(ctypes.POINTER(ctypes.c_float))))
 
        return preds
        
    def getScore(self):
        preds = np.zeros(self.dataset.data.shape[0], dtype=np.float32)
        _safe_call(rootGBM.getScore(self.dataset.Dataset_ptr, preds.ctypes.data_as(ctypes.POINTER(ctypes.c_float))))
        return preds
        
    def __del__(self):
        if self.Booster_ptr is not None:
            _safe_call(rootGBM.BoosterFree(self.Booster_ptr))
  

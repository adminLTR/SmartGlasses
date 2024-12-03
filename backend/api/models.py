from keras.applications.mobilenet import preprocess_input
from keras.preprocessing.image import img_to_array
from keras.preprocessing.image import load_img
from keras.models import load_model

from django.db import models
from django.core.validators import FileExtensionValidator
from PIL import Image
import numpy as np
import cv2
from django.utils.safestring import mark_safe
from django.core.files.base import ContentFile
import io
import tensorflow as tf
from tensorflow.keras.preprocessing.image import load_img, img_to_array
import pytesseract as pt

class Capture(models.Model): 
    frame = models.ImageField(upload_to="frames/", null=True)
    datetime = models.DateTimeField("Date and time of detection", auto_now_add=True, null=True)
    
    longitude = models.FloatField(null=True, blank=True)
    latitude = models.FloatField(null=True, blank=True)
    placa = models.CharField(null=True, blank=True, max_length=20)

    def save(self, *args, **kwargs):
        super().save(*args, **kwargs)
        if self.frame:
            self.OCR()

    def process_image(self):
        model = tf.keras.models.load_model("api/model.keras")

        image = load_img(self.frame.path) # PIL object
        image = np.array(image,dtype=np.uint8) # 8 bit array (0,255)
        image1 = load_img(self.frame.path,target_size=(224,224))

        # Redimensionar la imagen para la predicción
        image_arr_224 = img_to_array(image1)/255.0 # Convert to array & normalized
        h,w,d = image.shape
        test_arr = image_arr_224.reshape(1,224,224,3)

        # Hacer la predicción
        coords = model.predict(test_arr)
        
        # Denormalizar las coordenadas
        denorm = np.array([w, w, h, h])  # Para ajustar las coordenadas a la escala original
        coords = coords * denorm  # Multiplicar por las dimensiones de la imagen original
        coords = coords.astype(np.int32)  # Asegurarse de que las coordenadas son enteros
        
        return coords  # Devolver las coordenadas de la caja
        
    def OCR(self):
        img = np.array(load_img(self.frame.path))
        coords = self.process_image()
        xmin, xmax, ymin, ymax = coords[0]
        
        img = np.array(load_img(self.frame.path))
        xmin ,xmax,ymin,ymax = coords[0]
        roi = img[ymin-20:ymax+20,xmin-20:xmax+20]
        
        placa = pt.image_to_string(roi)

        self.placa = placa
        if self.placa is not None:                    
            super().save(update_fields=['placa'])
        

    def admin_image(self):
        return mark_safe('<a href="{url}"><img src="{url}" width="{width}" height={height} /></a>'.format(
            url=self.frame.url,
            width=100,
            height=100,
            ))
    admin_image.short_description = 'Image'
    admin_image.allow_tags = True

    class Meta:
        verbose_name = "Captura"
        verbose_name_plural = "Capturas"
  
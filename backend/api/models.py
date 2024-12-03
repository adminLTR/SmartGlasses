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

import imutils
import os

class Capture(models.Model): 
    frame = models.ImageField(upload_to="frames/", null=True)
    datetime = models.DateTimeField("Date and time of detection", auto_now_add=True, null=True)
    most_confident_label = models.CharField(max_length=50, blank=True, null=True)
    confidence = models.FloatField(null=True, blank=True)

    def save(self, *args, **kwargs):
        super().save(*args, **kwargs)
        if self.frame:
            self.process_image_for_detection()

    def process_image_for_detection(self):
        # Model architecture and weights
        #prototxt = "api/MobileNetSSD_deploy.prototxt.txt"
        #model = "api/MobileNetSSD_deploy.caffemodel"
        TARGET_SIZE = (224, 224)
        

        # Load the model
        #net = cv2.dnn.readNetFromCaffe(prototxt, model)
        model = load_model('api/hack_mobilenet.h5')

        # Preprocess the image for object detection
        open_cv_image = cv2.imread(self.frame.path)
        img = load_img(self.frame.path, target_size = TARGET_SIZE)
        img  = img_to_array(img)

        # our model was trained on RGB ordered images but OpenCV represents
        # images in BGR order, so swap the channels, and then resize to
        # 224x224 (the input dimensions for VGG16)
        open_cv_image = cv2.cvtColor(open_cv_image, cv2.COLOR_BGR2RGB)
        open_cv_image = cv2.resize(open_cv_image, TARGET_SIZE)

        # convert the image to a floating point data type and perform mean
        # subtraction
        open_cv_image = open_cv_image.astype("float32") / 255.
        img = np.expand_dims(img, axis = 0)
        img = preprocess_input(img)

        # Class labels
        waste_types = ['CARTON', 'VIDRIO', 'METAL', 'PAPEL', 'PLASTICO', 'GENERAL']

        # pass the image through the network to obtain our predictions
        # preds = model.predict(np.expand_dims(image / 255., axis=0))[0]
        preds = model.predict(img)[0]

        i = np.argmax(preds)
        label = waste_types[i]

        self.confidence = preds[i] * 100
        self.most_confident_label = label

        if self.confidence is not None:                    
            super().save(update_fields=['most_confident_label', 'confidence'])

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
  
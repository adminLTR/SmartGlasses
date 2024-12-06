
from django.db import models
from django.core.validators import FileExtensionValidator
from PIL import Image
import numpy as np
import cv2
from django.utils.safestring import mark_safe
from django.core.files.base import ContentFile
import io
from fast_alpr import ALPR

class Capture(models.Model): 
    frame = models.ImageField(upload_to="frames/", null=True)
    datetime = models.DateTimeField("Date and time of detection", auto_now_add=True, null=True)
    
    longitude = models.FloatField(null=True, blank=True)
    latitude = models.FloatField(null=True, blank=True)
    placa = models.CharField(null=True, blank=True, max_length=20)
    confidence = models.FloatField(null=True, blank=True)

    def save(self, *args, **kwargs):
        super().save(*args, **kwargs)
        if self.frame:
            self.detect_and_annotate_license_plate()

    def detect_and_annotate_license_plate(self):
        # Load the image
        image_path = self.frame.path
        alpr = ALPR(
            detector_model="yolo-v9-t-384-license-plate-end2end",
            ocr_model="european-plates-mobile-vit-v2-model",
        )
        frame = cv2.imread(image_path)
        if frame is None:
            raise FileNotFoundError(f"Image at {image_path} not found.")

        # Get predictions (plates and confidence)
        predictions = alpr.predict(frame)

        # List to store license plate details: (plate_number, confidence, bounding_box)
        license_plate_details = []
        detection_found = False

        # Draw predictions on the image
        for prediction in predictions:
            plate_number = prediction.ocr.text
            confidence = prediction.detection.confidence
            # Draw the rectangle around the plate
            rect = prediction.detection.bounding_box
            x1, y1, x2, y2 = rect.x1, rect.y1, rect.x2, rect.y2
            cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)  # Red rectangle

            # Add text with the plate number and confidence
            label = f"{plate_number} ({confidence:.2f})"

            # Create a blue background rectangle for the text
            text_size, _ = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.6, 2)
            text_width, text_height = text_size
            cv2.rectangle(frame, (x1, y1 - text_height - 5), (x1 + text_width, y1), (255, 0, 0), -1)  # Blue background
            cv2.putText(frame, label, (x1, y1 - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)  # White text

            # Store plate details
            license_plate_details.append({
                "plate_number": plate_number,
                "confidence": confidence,
                "bounding_box": (x1, y1, x2, y2)
            })

            detection_found = True

        # Save the annotated image
        cv2.imwrite(image_path, frame)

        if detection_found:
            self.placa = license_plate_details[0]["plate_number"]
            self.confidence = license_plate_details[0]["confidence"]

            super().save(update_fields=['placa', 'confidence'])


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
  
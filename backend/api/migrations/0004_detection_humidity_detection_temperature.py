# Generated by Django 5.0 on 2024-07-09 04:22

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0003_alter_detection_latitude_alter_detection_longitude'),
    ]

    operations = [
        migrations.AddField(
            model_name='detection',
            name='humidity',
            field=models.FloatField(blank=True, default=0, null=True),
        ),
        migrations.AddField(
            model_name='detection',
            name='temperature',
            field=models.FloatField(blank=True, default=0, null=True),
        ),
    ]

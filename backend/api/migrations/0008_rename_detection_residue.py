# Generated by Django 5.0 on 2024-07-09 05:22

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0007_alter_detection_trash_can'),
    ]

    operations = [
        migrations.RenameModel(
            old_name='Detection',
            new_name='Residue',
        ),
    ]

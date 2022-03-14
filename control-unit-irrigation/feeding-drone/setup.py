import setuptools

with open('README.md', 'r') as fh:
    long_description = fh.read()

setuptools.setup(
    name='feeding-drone',
    version='0.1.0',
    author='Marco Baldassarri - Francesco Vignola',
    author_email='marco.baldassarri2@studio.unibo.it - francesco.vignola@studio.unibo.it',
    description='A simple DJI Tello scripting in Python 3 for prototyping smart agricolture feeding using drones',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://bitbucket.org/unibo-projects/control-unit-irrigation',
    packages=setuptools.find_packages(),
    install_requires=[
        'opencv-python',
        'awsiotsdk'
    ],
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
)

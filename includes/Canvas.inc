<?php
class Canvas {
  protected $canvas_uri;
  protected $canvas_label;
  protected $thumbnail_uri;
  protected $resource_uri;
  protected $service_uri;
  protected $image_format;
  protected $image_height;
  protected $image_width;

  function __construct($canvas_uri, $label, $thumbnail_uri) {
    $this->canvas_uri = $canvas_uri;
    $this->canvas_label = $label;
    $this->thumbnail_uri = $thumbnail_uri;
  }

  function setResource($resource_uri, $service_uri, $format = 'image/jpeg', $width, $height) {
    $this->resource_uri = $resource_uri;
    $this->service_uri = $service_uri;
    $this->image_format = $format;
    $this->image_width = intval($width);
    $this->image_height = intval($height);
  }

  function toArray() {
    $returnArray = array (
        '@type' => 'sc:Canvas',
        '@id' => $this->canvas_uri,
        'label' => $this->canvas_label,
        'height' => $this->image_height,
        'width' => $this->image_width,
        'thumbnail' => array (
            '@id' => $this->thumbnail_uri,
            'service' => array (
                '@context' => 'http://iiif.io/api/image/2/context.json',
                '@id' => $this->service_uri,
                'profile' => 'http://iiif.io/api/image/2/level2.json'
            )
        ),
        'images' => array ( // always contain only one
            array (
                '@type' => 'oa:Annotation',
                'motivation' => 'sc:Painting',
                'on' => $this->canvas_uri,
                'resource' => array (
                    '@id' => $this->resource_uri,
                    '@type' => 'dctypes:Image',
                    'format' => $this->image_format,
                    'height' => $this->image_height,
                    'width' => $this->image_width,
                    'service' => array (
                        '@context' => 'http://iiif.io/api/image/2/context.json',
                        '@id' => $this->service_uri,
                        'profile' => 'http://iiif.io/api/image/2/level2.json'
                     )
                )
            )
        )
    );

    return $returnArray;
  }
}

<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * To get locations of building Objects.
 * 
 * @param array $buildings_pid
 *   The array contains pids of buildings.  
 *
 * @return \stdClass
 *   Locations to be shown on the map.
 */
function delftdora_get_building_locations(array $buildings_pid) {
  $locations = array();
  foreach ($buildings_pid as $building_pid) {
    $building = islandora_object_load($building_pid);
    if (in_array('model:Building', $building->models)) {
      $dc_content = $building['DC']->content;
      $dc_xml = new SimpleXMLElement($dc_content);
      $dc_xml->registerXPathNamespace('dc', 'http://purl.org/dc/elements/1.1/');
      $locations_array = $dc_xml->xpath('//dc:coverage');
      foreach ($locations_array as $loaction_string) {
        if (preg_match('*,*', $loaction_string)) {
          $position = explode(',', $loaction_string);
          $location = new stdClass();
          $location->latitude = floatval($position[0]);
          $location->longitude = floatval($position[1]);
          $location->balloon_text = $building->label;
          $locations[] = $location;
        }
      }
    }
  }
  return $locations;
}

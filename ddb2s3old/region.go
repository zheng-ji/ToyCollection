package main

import (
	"github.com/AdRoll/goamz/aws"
)

var Regions = map[string]aws.Region{
	aws.APNortheast.Name:  aws.APNortheast,
	aws.APSoutheast.Name:  aws.APSoutheast,
	aws.APSoutheast2.Name: aws.APSoutheast2,
	aws.EUWest.Name:       aws.EUWest,
	aws.USEast.Name:       aws.USEast,
	aws.USWest.Name:       aws.USWest,
	aws.USWest2.Name:      aws.USWest2,
	aws.USGovWest.Name:    aws.USGovWest,
	aws.SAEast.Name:       aws.SAEast,
	aws.CNNorth1.Name:     aws.CNNorth1,
}
